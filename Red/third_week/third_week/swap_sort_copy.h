#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>


template <typename T>
void Swap(T* first, T* second)
{
	std::swap(*first, *second);
}


template <typename T>
void SortPointers(std::vector<T*>& pointers)
{
    if (pointers.empty())
    {
        return;
    }

	std::multiset<T> tmp_set;

	for (auto item : pointers)
	{
		tmp_set.insert(*item);
	}

	size_t i = 0;
    for (T item : tmp_set)
    {
		*pointers[i] = item;
		++i;
    }
}


template <typename T>
void ReversedCopy(T* source, size_t count, T* destination)
{
    if (count == 0)
        return;

	std::vector<T> tmp_vec(count);
	size_t i = count - 1;

    for (T& item : tmp_vec)
    {
		item = source[i];
		--i;
    }

	i = 0;
    for (T item : tmp_vec)
    {
		destination[i] = item;
		++i;
    }
}


void TestSwapSortCopy();