#include "UnitTestsFramework.h"
#include <algorithm>
#include <memory>
#include <vector>
#include "merge_sort.h"
#include <numeric>


void TestSortWithVectorInt() 
{
	{
		std::vector<int> numbers = { 6, 1, 3, 9, 1, 9, 8, 12, 1 };
		MergeSort(begin(numbers), end(numbers));
		ASSERT(is_sorted(begin(numbers), end(numbers)));
	}

	{
		std::vector<int> numbers(59049);
		std::iota(numbers.rbegin(), numbers.rend(), 1);

		MergeSort(begin(numbers), end(numbers));
		ASSERT(is_sorted(begin(numbers), end(numbers)));
	}
}


void TestMergeSort() 
{
	TestRunner tr;
	RUN_TEST(tr, TestSortWithVectorInt);
}