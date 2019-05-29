#include "UnitTestsFramework.h"
#include "swap_sort_copy.h"


void TestSwap() 
{
    int a = 1;
	int b = 2;
	Swap(&a, &b);
	ASSERT_EQUAL(a, 2);
	ASSERT_EQUAL(b, 1);

    std::string h = "world";
    std::string w = "hello";
	Swap(&h, &w);
	ASSERT_EQUAL(h, "hello");
	ASSERT_EQUAL(w, "world");
}


void TestSortPointers() 
{
	int one = 1;
	int two = 2;
	int three = 3;

    std::vector<int*> pointers;
	pointers.push_back(&two);
	pointers.push_back(&three);
	pointers.push_back(&one);

	SortPointers(pointers);

	ASSERT_EQUAL(pointers.size(), 3u);
	ASSERT_EQUAL(*pointers[0], 1);
	ASSERT_EQUAL(*pointers[1], 2);
	ASSERT_EQUAL(*pointers[2], 3);

	std::vector<int*> v;
	std::vector<int> tmp_vec;
    for (int i = 0; i < 6; ++i)
    {
		tmp_vec.push_back(5 - i);
		tmp_vec.push_back(i);
    }
    for (int& item : tmp_vec)
    {
		v.push_back(&item);
    }
	SortPointers(v);

	int expected = 0;
    for (int i = 0; i < 12; i += 2)
    {
		ASSERT_EQUAL(*v[i], expected);
		ASSERT_EQUAL(*v[i + 1], expected);
		++expected;
    }
}


void TestReverseCopy() {
	const int count = 7;

	int* source = new int[count];
	int* dest = new int[count];

	for (int i = 0; i < count; ++i) 
	{
		source[i] = i + 1;
	}

	ReversedCopy(source, count, dest);
	const std::vector<int> expected1 = { 7, 6, 5, 4, 3, 2, 1 };
	ASSERT_EQUAL(std::vector<int>(dest, dest + count), expected1);

	// Области памяти могут перекрываться
	ReversedCopy(source, count - 1, source + 1);
	const std::vector<int> expected2 = { 1, 6, 5, 4, 3, 2, 1 };
	ASSERT_EQUAL(std::vector<int>(source, source + count), expected2);

	delete[] dest;
	delete[] source;
}


void TestSwapSortCopy()
{
	TestRunner tr;
	RUN_TEST(tr, TestSwap);
	RUN_TEST(tr, TestSortPointers);
	RUN_TEST(tr, TestReverseCopy);
}