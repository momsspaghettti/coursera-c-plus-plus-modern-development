#pragma once
#include <algorithm>
#include <iterator>


template <typename It>
class IteratorRange {
public:
    IteratorRange(It first, It last) : first(first), last(last) {
    }

    It begin() const {
        return first;
    }

    It end() const {
        return last;
    }

    [[nodiscard]] size_t size() const {
        return std::distance(first, last);
    }

private:
    It first, last;
};

template <typename Container>
auto Head(Container& c, int top) {
    return IteratorRange(std::begin(c), std::begin(c) + std::min<size_t>(std::max(top, 0), c.size()));
}