#include "cell.h"
#include "sheet.h"

#include <cassert>
#include <optional>
#include <stack>

class Cell::Impl {
public:
    virtual ~Impl() = default;

    virtual Value GetValue() const = 0;

    virtual std::string GetText() const = 0;

    virtual std::vector<Position> GetReferencedCells() const {
        return {};
    }

    virtual IFormula *GetFormula() {
        return nullptr;
    }

    virtual bool IsCacheValid() const {
        return true;
    }

    virtual void InvalidateCache() {}
};

class Cell::EmptyImpl : public Impl {
public:
    Value GetValue() const override {
        return "";
    }

    std::string GetText() const override {
        return "";
    }
};

class Cell::TextImpl : public Impl {
public:
    explicit TextImpl(std::string text) : text_(std::move(text)) {
        if (text_.empty()) {
            throw std::logic_error(
                    "TextImpl should not contain empty text, use EmptyImpl for this "
                    "purpose.");
        }
    }

    Value GetValue() const override {
        if (text_[0] == kEscapeSign) {
            return text_.substr(1);
        }
        return text_;
    }

    std::string GetText() const override {
        return text_;
    }

private:
    std::string text_;
};

class Cell::FormulaImpl : public Impl {
public:
    explicit FormulaImpl(std::string text, const ISheet &sheet) : sheet_(sheet) {
        if (text.empty() || text[0] != kFormulaSign) {
            throw std::logic_error("A formula should start with '=' sign");
        }
        formula_ = ParseFormula(text.substr(1));
    }

    Value GetValue() const override {
        if (!cachedValue_) {
            cachedValue_ = formula_->Evaluate(sheet_);
        }
        return std::visit([](const auto &x) { return Value(x); }, *cachedValue_);
    }

    std::string GetText() const override {
        return kFormulaSign + formula_->GetExpression();
    }

    std::vector<Position> GetReferencedCells() const override {
        return formula_->GetReferencedCells();
    }

    IFormula *GetFormula() override {
        return formula_.get();
    }

    bool IsCacheValid() const override {
        return cachedValue_.has_value();
    }

    void InvalidateCache() override {
        cachedValue_.reset();
    }

private:
    const ISheet &sheet_;
    std::unique_ptr<IFormula> formula_;
    mutable std::optional<IFormula::Value> cachedValue_;
};

Cell::Cell(Sheet &sheet) :
        impl_(std::make_unique<EmptyImpl>()),
        sheet_(sheet) {}

Cell::~Cell() {
    // Если ячейку удаляют, когда на неё существуют ссылки, то соответствующие
    // указатели станут висячими. Но в данном месте мы уже ничего не можем с этим
    // поделать.
    assert(incomingRefs_.empty());
    assert(outgoingRefs_.empty());
}

void Cell::Set(std::string text) {
    std::unique_ptr<Impl> newImpl;
    if (text.empty()) {
        newImpl = std::make_unique<EmptyImpl>();
    } else if (text.size() > 1 && text[0] == kFormulaSign) {
        newImpl = std::make_unique<FormulaImpl>(std::move(text), sheet_);
    } else {
        newImpl = std::make_unique<TextImpl>(std::move(text));
    }

    if (WouldIntroduceCircularDependency(*newImpl)) {
        throw CircularDependencyException(
                "Setting this formula would introduce circular dependency!");
    }

    impl_ = std::move(newImpl);

    UpdateRefs();

    // Запускаем инвалидацию с флагом force, т.к. наше собственное значение только
    // что было создано, и нам не важно, есть там кеш или нет, но все ячейки, что
    // от нас зависят, точно нужно инвалидировать
    // TODO do not force invalidate in case the cell was already invalid before
    // the call
    InvalidateCacheRecursive(true);
}

void Cell::Clear() {
    Set("");
}

Cell::Value Cell::GetValue() const {
    return impl_->GetValue();
}

std::string Cell::GetText() const {
    return impl_->GetText();
}

std::vector<Position> Cell::GetReferencedCells() const {
    return impl_->GetReferencedCells();
}

void Cell::UpdateFormula(
        const std::function<IFormula::HandlingResult(IFormula &)> &update) {
    if (IFormula *formula = impl_->GetFormula()) {
        auto result = update(*formula);
        if (result == IFormula::HandlingResult::ReferencesChanged) {
            UpdateRefs();
        }
    }
}

bool Cell::IsReferenced() const {
    return !incomingRefs_.empty();
}

bool Cell::WouldIntroduceCircularDependency(const Impl &newImpl) const {
    if (newImpl.GetReferencedCells().empty()) {
        return false;
    }

    std::unordered_set<const Cell *> referenced;
    for (const auto &pos : newImpl.GetReferencedCells()) {
        referenced.insert(sheet_.GetConcreteCell(pos));
    }

    std::unordered_set<const Cell *> visited;
    std::stack<const Cell *> toVisit;
    toVisit.push(this);
    while (!toVisit.empty()) {
        const Cell *current = toVisit.top();
        toVisit.pop();
        visited.insert(current);
        if (referenced.find(current) != referenced.end()) {
            return true;
        }
        for (const Cell *incoming : current->incomingRefs_) {
            if (visited.find(incoming) == visited.end()) {
                toVisit.push(incoming);
            }
        }
    }

    return false;
}

void Cell::UpdateRefs() {
    for (Cell *outgoing : outgoingRefs_) {
        outgoing->incomingRefs_.erase(this);
    }
    outgoingRefs_.clear();
    for (const auto &pos : impl_->GetReferencedCells()) {
        Cell *outgoing = sheet_.GetConcreteCell(pos);
        if (!outgoing) {
            sheet_.SetCell(pos, "");
            outgoing = sheet_.GetConcreteCell(pos);
        }
        outgoingRefs_.insert(outgoing);
        outgoing->incomingRefs_.insert(this);
    }
}

void Cell::InvalidateCacheRecursive(bool force) {
    if (impl_->IsCacheValid() || force) {
        impl_->InvalidateCache();
        // Если ячейка текстовая, то она всё равно останется валидной, но она
        // не может ссылаться на другие ячейки, значит она может быть только
        // листом в графе зависимостей, и последующий цикл в неё не зайдёт
        // повторно
        for (Cell *incoming : incomingRefs_) {
            incoming->InvalidateCacheRecursive();
        }
    }
}
