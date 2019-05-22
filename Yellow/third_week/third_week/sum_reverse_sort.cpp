#include "pch.h"
#include "sum_reverse_sort.h"
#include <algorithm>


int Sum(int x, int y)
{
	return x + y;
}


string Reverse(string s)
{
	std::reverse(s.begin(), s.end());

	return s;
}


void Sort(vector<int>& nums)
{
	std::sort(nums.begin(), nums.end());
}


