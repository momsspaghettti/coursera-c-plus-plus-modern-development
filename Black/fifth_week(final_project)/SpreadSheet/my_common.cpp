#include "common.h"


bool Position::operator==(const Position &rhs) const {
    return rhs.row == row && rhs.col == col;
}

bool Position::operator<(const Position &rhs) const {
    return row < rhs.row || col < rhs.col;
}

bool Position::IsValid() const {
    return (row > -1 && row < kMaxRows) &&
           (col > -1 && col < kMaxCols);
}

const std::vector<char> index_to_letter_ = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};

std::string Position::ToString() const {
    std::string result;
    if (IsValid()) {
        auto col_copy = col;
        bool first = true;
        do {
            if (first) {
                first = false;
            } else {
                --col_copy;
            }
            auto residue = col_copy % 26;
            result = index_to_letter_.at(residue) + result;
            col_copy /= 26;
        } while (col_copy != 0);
        result += std::to_string(row + 1);
    } else {
        result = "";
    }
    return result;
}

Position Position::FromString(std::string_view str) {
    if (str.empty()) {
        return Position{-1, -1};
    } else if (
        int i = str[0] - 'A';
            i < 0 || i >= index_to_letter_.size()
            ) {
        return Position{-1, -1};
    } else {
        int row = 0;
        int col = 0;
        size_t index = 0;
        int idx = str[index] - 'A';
        bool first = true;
        while (
                idx >= 0 && idx < index_to_letter_.size() &&
                index < str.size()
                ) {
            idx = str[index] - 'A';
            if (idx < 0 || idx >= index_to_letter_.size())
                break;
            if (first) {
                first = false;
            } else {
                ++col;
            }
            col *= 26;
            col += idx;
            ++index;
        }
        if (index == str.size()) {
            return Position{-1, -1};
        } else if (!std::isdigit(str[index])) {
            return Position{-1, -1};
        } else if (str[index] - '0' == 0) {
            return Position{-1, -1};
        } else {
            char tmp = str[index];
            while (
                    std::isdigit(tmp) &&
                    index < str.size()
                    ) {
                tmp = str[index];
                if (!std::isdigit(tmp))
                    break;
                row *= 10;
                row += tmp - '0';
                ++index;
            }
            if (index != str.size()) {
                return Position{-1, -1};
            }
            return Position{row - 1, col};
        }
    }
}


bool Size::operator==(const Size &rhs) const {
    return rows == rhs.rows && cols == rhs.cols;
}


FormulaError::FormulaError(FormulaError::Category category) {
    category_ = category;
}

FormulaError::Category FormulaError::GetCategory() const {
    return category_;
}

bool FormulaError::operator==(FormulaError rhs) const {
    return category_ == rhs.category_;
}

std::string_view FormulaError::ToString() const {
    switch (category_) {
        case Category::Ref:
            return "#REF!";
        case Category::Value:
            return "#VALUE!";
        case Category::Div0:
            return "#DIV/0!";
        default:
            return "";
    }
}

std::ostream &operator<<(std::ostream &output, FormulaError fe) {
    output << fe.ToString();
    return output;
}