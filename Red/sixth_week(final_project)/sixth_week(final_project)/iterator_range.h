#pragma once
#include <algorithm>


template <typename It>
class IteratorRange 
{
public:
    IteratorRange(It first, It last) : first(first), last(last) {}

    It begin() const 
    {
        return first;
    }

    It end() const 
    {
        return last;
    }

    size_t size() const 
    {
        return last - first;
    }

private:
    It first, last;
};


template <typename Container>
auto Head(Container& c, int top) 
{
    return IteratorRange(begin(c), begin(c) + 
		std::min<size_t>(std::max(top, 0), c.size()));
}