#pragma once
#include <iostream>
#include <algorithm>


template <typename Iterator >
class IteratorRange
{
private:
	Iterator first_, last_;

public:
	IteratorRange<Iterator>(Iterator first, Iterator last)
	{
		first_ = first;
		last_ = last;
	}

	Iterator begin() const 
    {
		return first_;
	}

	Iterator end() const 
    {
		return last_;
	}

    size_t size() const
	{
		return static_cast<size_t>(last_ - first_);
	}
};


template <typename Iterator>
class Paginator
{
public:
	Paginator<Iterator>(Iterator begin, Iterator end, const size_t& page_size)
	{
	    const size_t size = end - begin;

        if (page_size >= size)
        {
			pages_.push_back(IteratorRange<Iterator>(begin, end));
            return;
        }

		auto left = begin;
		auto right = begin;

        while (right != end)
        {
			left = right;
			right += std::min(page_size, static_cast<size_t>(end - right));

            pages_.push_back(IteratorRange<Iterator>(left, right));
        }
	}

    auto begin() const
	{
		return pages_.begin();
	}

    auto end() const
	{
		return pages_.end();
	}

    size_t size() const
	{
		return pages_.size();
	}

private:
	std::vector<IteratorRange<Iterator>> pages_;
};


template <typename C>
auto Paginate(C& c, size_t page_size)
{
	return Paginator(c.begin(), c.end(), page_size);
}


void TestPaginator();