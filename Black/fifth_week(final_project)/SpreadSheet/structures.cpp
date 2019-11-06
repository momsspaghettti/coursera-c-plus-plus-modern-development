#include "common.h"

#include <cctype>
#include <sstream>

const int kLetters = 26;
const std::string kMaxColStr = "XFD";

bool Position::operator==(const Position &rhs) const {
    return row == rhs.row && col == rhs.col;
}

bool Position::operator<(const Position &rhs) const {
    return std::tie(row, col) < std::tie(rhs.row, rhs.col);
}

bool Position::IsValid() const {
    return row >= 0 && col >= 0 && row < kMaxRows && col < kMaxCols;
}

std::string Position::ToString() const {
    if (!IsValid()) {
        return "";
    }

    std::string result;
    result.reserve(17);
    int c = col;
    while (c >= 0) {
        result.insert(result.begin(), 'A' + c % kLetters);
        c = c / kLetters - 1;
    }

    result += std::to_string(row + 1);

    return result;
}

Position Position::FromString(std::string_view str) {
    auto it = str.begin();
    while (it != str.end() && std::isalpha(*it) && std::isupper(*it)) {
        ++it;
    }
    auto letters = str.substr(0, it - str.begin());
    auto digits = str.substr(it - str.begin());

    auto result = Position{-1, -1};
    if (letters.empty() || digits.empty()) {
        return result;
    }
    if (letters.size() > kMaxColStr.size()) {
        return result;
    }

    if (!std::isdigit(digits[0])) {
        return result;
    }

    int row;
    std::istringstream row_in{std::string{digits}};
    if (!(row_in >> row) || !row_in.eof()) {
        return result;
    }

    int col = 0;
    for (char ch : letters) {
        col *= kLetters;
        col += ch - 'A' + 1;
    }

    result.row = row - 1;
    result.col = col - 1;

    return result;
}

bool Size::operator==(const Size &rhs) const {
    return cols == rhs.cols && rows == rhs.rows;
}
