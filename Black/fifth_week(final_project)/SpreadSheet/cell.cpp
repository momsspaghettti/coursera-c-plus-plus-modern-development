#include "cell.h"
#include "formula.h"
#include <utility>


/*Cell::Cell(std::string text, ISheet* sheet) {
    text_ = std::move(text);
    interpret_text(sheet);
}

void Cell::interpret_text(ISheet* sheet) {
    if (text_.empty()) {
        value_ = 0;
    } else if (text_.front() == '\'') {
        value_ = std::string(text_.begin() + 1, text_.end());
    } else if (text_.front() != '=' || (text_.front() == '=' && text_.size() == 1)) {
        value_ = text_;
    } else {
        try {
            auto formula_val = ParseFormula(text_)->Evaluate(*sheet);
        } catch (FormulaException&) {
            throw;
        }
    }
}*/