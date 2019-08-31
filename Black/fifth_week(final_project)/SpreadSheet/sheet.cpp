#include "sheet.h"
#include <memory>
#include <algorithm>


Sheet::Sheet() {
    sheet_ = std::make_unique<std::vector<std::vector<std::shared_ptr<Cell>>>>();
    sheet_->reserve(Position::kMaxRows);
    size_ = Size{0, 0};
    cache_ = std::make_unique<std::unordered_map<Position, PositionCache, PositionHash>>();
    cache_->reserve(1000 * Position::kMaxRows);
}

Size Sheet::GetPrintableSize() const {
    return size_;
}

void Sheet::SetCell(Position pos, std::string text) {
    if (pos.IsValid()) {
        const auto finder = cache_->find(pos);
        if (finder != cache_->end()) {
            if (*finder->second.getCache() != text) {
                const auto new_cell = update_cell(pos, text);
                (*sheet_)[pos.row][pos.col] = new_cell;
            }
        } else {
            Size new_size = Size{
                    std::max(size_.rows, pos.row + 1),
                    std::max(size_.cols, pos.col + 1)
            };
            if (!(new_size == size_)) {
                sheet_->resize(new_size.rows);
                for (size_t i = 0; i < new_size.cols; ++i) {
                    sheet_->at(i).resize(new_size.cols);
                }
                size_ = new_size;
                const auto new_cell = create_new_cell(pos, text);
                (*sheet_)[pos.row][pos.col] = new_cell;
            }
        }
    } else {
        throw InvalidPositionException("Invalid Position in SetCell Method!");
    }
}

std::shared_ptr<Cell> Sheet::create_new_cell(
        const Position &pos,
        const std::string &text
) {

}

std::shared_ptr<Cell> Sheet::update_cell(
        const Position &pos,
        const std::string &text
) {

}

const ICell *Sheet::GetCell(Position pos) const {
    if (pos.IsValid()) {
        return nullptr;
    } else {
        throw InvalidPositionException("Invalid Position in GetCell Method!");
    }
}

ICell *Sheet::GetCell(Position pos) {
    if (pos.IsValid()) {
        return nullptr;
    } else {
        throw InvalidPositionException("Invalid Position in GetCell Method!");
    }
}

void Sheet::ClearCell(Position pos) {
    if (pos.IsValid()) {

    } else {
        throw InvalidPositionException("Invalid Position in ClearCell Method!");
    }
}

void Sheet::InsertCols(int before, int count) {

}

void Sheet::InsertRows(int before, int count) {

}

void Sheet::DeleteCols(int first, int count) {

}

void Sheet::DeleteRows(int first, int count) {

}

void Sheet::PrintValues(std::ostream &output) const {

}

void Sheet::PrintTexts(std::ostream &output) const {

}


std::unique_ptr<ISheet> CreateSheet() {
    return std::make_unique<Sheet>();
}