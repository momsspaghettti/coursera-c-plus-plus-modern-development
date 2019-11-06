#include "common.h"

#include "sheet.h"
#include "cell.h"
#include "formula.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

namespace {
    template<class Range>
    int ssize(const Range &range) {
        return static_cast<int>(std::size(range));
    }

    // Вставляет в вектор перед указанным элементом заданное количество элементов,
    // которые будут сконструированы по умолчанию. К сожалению,
    // std::vector::insert() этого сделать не позволяет.
    // Если вставка должна происходить в конец, то не делает ничего.
    // Возвращает диапазон вставленных элементов.
    template<class T>
    auto InsertToVectorDefaultConstructed(std::vector<T> &v, int before,
                                          int count) {
        if (before >= ssize(v)) {
            return std::pair(v.end(), v.end());
        }
        v.resize(v.size() + count);
        auto first = v.begin() + before;
        auto last = v.end() - count;
        std::move_backward(first, last, v.end());
        return std::pair(first, first + count);
    }

    template<class T>
    void SafeErase(std::vector<T> &v, int first, int count) {
        if (first < ssize(v)) {
            count = std::min(count, ssize(v) - first);
            v.erase(v.begin() + first, v.begin() + first + count);
        }
    }
}

Sheet::~Sheet() {
    // Сначала очистим ячейки во избежания возникновения висячих указателей в
    // процессе удаления. Критической необходимости в этом нет, но так безопаснее.
    for (auto &row : cells_) {
        for (auto &cell : row) {
            if (cell) {
                cell->Clear();
            }
        }
    }
}

void Sheet::SetCell(Position pos, std::string text) {
    if (!pos.IsValid()) {
        throw InvalidPositionException(
                "Invalid position passed to Sheet::SetCell()");
    }
    MaybeIncreaseSizeToIncludePosition(pos);
    auto &cell = cells_[pos.row][pos.col];
    if (!cell) {
        cell = std::make_unique<Cell>(*this);
    }
    cell->Set(std::move(text));
}

const ICell *Sheet::GetCell(Position pos) const {
    return GetConcreteCell(pos);
}

ICell *Sheet::GetCell(Position pos) {
    return GetConcreteCell(pos);
}

void Sheet::ClearCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException(
                "Invalid position passed to Sheet::ClearCell()");
    }
    if (pos.row < ssize(cells_) && pos.col < ssize(cells_[pos.row])) {
        if (auto &cell = cells_[pos.row][pos.col]) {
            cell->Clear();
            if (!cell->IsReferenced()) {
                cell.reset();
            }
        }
    }
}

void Sheet::InsertRows(int before, int count) {
    if (count == 0) {
        return;
    }
    if (before < 0 || count < 0) {
        throw std::out_of_range("Wrong arguments for InsertRows()");
    }
    if (GetActualSize().rows + count >= Position::kMaxRows) {
        throw TableTooBigException(
                "Adding rows would push some cells out of allowed table bounds");
    }
    InsertToVectorDefaultConstructed(cells_, before, count);
    UpdateFormulas([=](IFormula &formula) {
        return formula.HandleInsertedRows(before, count);
    });
}

void Sheet::InsertCols(int before, int count) {
    if (count == 0) {
        return;
    }
    if (before < 0 || count < 0) {
        throw std::out_of_range("Wrong arguments for InsertCols()");
    }
    if (GetActualSize().cols + count >= Position::kMaxCols) {
        throw TableTooBigException(
                "Adding cols would push some cells out of allowed table bounds");
    }
    for (auto &row : cells_) {
        InsertToVectorDefaultConstructed(row, before, count);
    }
    UpdateFormulas([=](IFormula &formula) {
        return formula.HandleInsertedCols(before, count);
    });
}

void Sheet::DeleteRows(int first, int count) {
    if (count == 0) {
        return;
    }
    if (first < 0 || count < 0) {
        throw std::out_of_range("Wrong arguments for DeleteRows()");
    }

    std::vector<std::unique_ptr<Cell>> cellsToRemove;

    // Прежде чем удалять ячейки, нужно убедиться, что у них нет ни входящих, ни
    // исходящих ссылок (иначе у нас появятся висячие указатели). Очистка ячеек
    // (вызов метода Clear) позволит убрать все исходящие ссылки. Со входящими
    // ссылками дело обстоит интереснее. Они будут убраны автоматически, когда
    // обновятся исходящие ссылки ячеек, которые ссылаются на удаляемые. Это
    // произойдёт при обновлении формул. Но обновление формул может привести к
    // запросу ячеек с некоторой позицией (ведь именно так задаются ячейки в
    // формулах). Значит, чтобы получить в ответ правильную ячейку, элементы
    // таблицы уже должны быть в этот момент удалены. Но ячейки-то удалять пока
    // нельзя, мы с этого начали! Получается, что нужно на время извлечь ячейки из
    // таблицы, и удалить их в самом конце, после обновления формул.
    for (int i = 0; i < count; ++i) {
        int r = first + i;
        if (r < ssize(cells_)) {
            for (auto &cell : cells_[r]) {
                if (cell) {
                    cell->Clear();
                    cellsToRemove.emplace_back(std::move(cell));
                }
            }
        }
    }

    // Важно удалить элементы таблицы до того, как обновлять формулы, чтобы
    // обновлённые индексы указывали на правильные ячейки
    SafeErase(cells_, first, count);

    // Теперь обновим формулы
    UpdateFormulas([=](IFormula &formula) {
        return formula.HandleDeletedRows(first, count);
    });

    // А вот теперь уже можно удалять ячейки, что и будет сделано неявно в
    // деструкторе объекта cellsToRemove
}

void Sheet::DeleteCols(int first, int count) {
    if (count == 0) {
        return;
    }
    if (first < 0 || count < 0) {
        throw std::out_of_range("Wrong arguments for DeleteCols()");
    }

    // См. комментарии в теле DeleteRows(), здесь идея та же самая
    std::vector<std::unique_ptr<Cell>> cellsToRemove;

    for (auto &row : cells_) {
        for (int i = 0; i < count; ++i) {
            int c = first + i;
            if (c < ssize(row)) {
                if (auto &cell = row[c]) {
                    cell->Clear();
                    cellsToRemove.emplace_back(std::move(cell));
                }
            }
        }
    }

    for (auto &row : cells_) {
        SafeErase(row, first, count);
    }

    UpdateFormulas([=](IFormula &formula) {
        return formula.HandleDeletedCols(first, count);
    });
}

Size Sheet::GetPrintableSize() const {
    Size size;
    for (int row = 0; row < ssize(cells_); ++row) {
        for (int col = ssize(cells_[row]) - 1; col >= 0; --col) {
            if (const auto &cell = cells_[row][col]) {
                if (!cell->GetText().empty()) {
                    size.rows = std::max(size.rows, row + 1);
                    size.cols = std::max(size.cols, col + 1);
                    break;
                }
            }
        }
    }
    return size;
}

void Sheet::PrintValues(std::ostream &output) const {
    PrintCells(output, [&](const ICell &cell) {
        std::visit([&](const auto &value) { output << value; }, cell.GetValue());
    });
}

void Sheet::PrintTexts(std::ostream &output) const {
    PrintCells(output,
               [&output](const ICell &cell) { output << cell.GetText(); });
}

const Cell *Sheet::GetConcreteCell(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException(
                "Invalid position passed to Sheet::GetCell()");
    }
    if (pos.row >= ssize(cells_) || pos.col >= ssize(cells_[pos.row])) {
        return nullptr;
    }
    return cells_[pos.row][pos.col].get();
}

Cell *Sheet::GetConcreteCell(Position pos) {
    return const_cast<Cell *>(
            static_cast<const Sheet &>(*this).GetConcreteCell(pos));
}

void Sheet::MaybeIncreaseSizeToIncludePosition(Position pos) {
    cells_.resize(std::max(pos.row + 1, ssize(cells_)));
    cells_[pos.row].resize(std::max(pos.col + 1, ssize(cells_[pos.row])));
}

void Sheet::PrintCells(
        std::ostream &output,
        const std::function<void(const ICell &)> &printCell) const {
    auto size = GetPrintableSize();
    for (int row = 0; row < size.rows; ++row) {
        for (int col = 0; col < size.cols; ++col) {
            if (col > 0) {
                output << '\t';
            }
            if (col < ssize(cells_[row])) {
                if (const auto &cell = cells_[row][col]) {
                    printCell(*cell);
                }
            }
        }
        output << '\n';
    }
}

void Sheet::UpdateFormulas(
        const std::function<IFormula::HandlingResult(IFormula &)> &update) {
    // TODO: keep track of current formulas to optimize search
    for (auto &row : cells_) {
        for (auto &cell : row) {
            if (cell) {
                cell->UpdateFormula(update);
            }
        }
    }
}

Size Sheet::GetActualSize() const {
    Size size;
    for (int row = 0; row < ssize(cells_); ++row) {
        for (int col = ssize(cells_[row]) - 1; col >= 0; --col) {
            if (cells_[row][col]) {
                size.rows = std::max(size.rows, row + 1);
                size.cols = std::max(size.cols, col + 1);
                break;
            }
        }
    }
    return size;
}

std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}
