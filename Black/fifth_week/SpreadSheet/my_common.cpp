#include "common.h"


bool Position::operator==(const Position &rhs) const {
    return rhs.row == row && rhs.col == col;
}

bool Position::operator<(const Position &rhs) const {
    return row < rhs.row || col < rhs.col;
}

bool Position::IsValid() const {
    return is_valid(row, col);
}

bool Position::is_valid(int r, int c) {
    return (r > -1 && r < kMaxRows) &&
           (c > -1 && c < kMaxCols);
}

const std::vector<char> Position::index_to_letter_ = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};

const std::unordered_map<char, size_t> Position::letter_to_index_ = {
        {'A', 0},
        {'B', 1},
        {'C', 2},
        {'D', 3},
        {'E', 4},
        {'F', 5},
        {'G', 6},
        {'H', 7},
        {'I', 8},
        {'J', 9},
        {'K', 10},
        {'L', 11},
        {'M', 12},
        {'N', 13},
        {'O', 14},
        {'P', 15},
        {'Q', 16},
        {'R', 17},
        {'S', 18},
        {'T', 19},
        {'U', 20},
        {'V', 21},
        {'W', 22},
        {'X', 23},
        {'Y', 24},
        {'Z', 25}
};

std::string Position::ToString() const {
    std::string result;
    if (IsValid()) {
        int col_copy = col;
        do {
            const auto residue = col_copy % 26;
            result = index_to_letter_.at(residue) + result;
            col_copy -= 26;
        } while (col_copy >= 0);
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
        const auto f = letter_to_index_.find(str[0]);
            f == letter_to_index_.end()
            ) {
        return Position{-1, -1};
    } else {
        int row = 0;
        int col = 0;
        size_t index = 0;
        auto finder = letter_to_index_.find(str[index]);
        while (
                finder != letter_to_index_.end() &&
                index < str.size()
                ) {
            finder = letter_to_index_.find(str[index]);
            if (finder == letter_to_index_.end())
                break;
            col += finder->second;
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
                row += tmp -'0';
                ++index;
            }
            if (index != str.size()) {
                return Position{-1, -1};
            }
            return Position{row - 1, col};
        }
    }
}