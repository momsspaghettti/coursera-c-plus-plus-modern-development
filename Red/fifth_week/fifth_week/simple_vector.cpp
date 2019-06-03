#include "simple_vector.h"
#include "UnitTestsFramework.h"
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>


void TestConstruction() 
{
    SimpleVector<int> empty;
    ASSERT_EQUAL(empty.Size(), 0u);
    ASSERT_EQUAL(empty.Capacity(), 0u);
    ASSERT(empty.begin() == empty.end());

    SimpleVector<std::string> five_strings(5);
    ASSERT_EQUAL(five_strings.Size(), 5u);
    ASSERT(five_strings.Size() <= five_strings.Capacity());
    for (auto& item : five_strings) 
	{
        ASSERT(item.empty());
	}
  
    five_strings[2] = "Hello";
    ASSERT_EQUAL(five_strings[2], "Hello");
}


void TestPushBack() 
{
    SimpleVector<int> v;

    for (int i = 10; i >= 1; --i) 
	{
        v.PushBack(i);
        ASSERT(v.Size() <= v.Capacity());
    }
    std::sort(v.begin(), v.end());

    const std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_EQUAL(v.Size(), expected.size());
    ASSERT(equal(v.begin(), v.end(), expected.begin()));
}


void TestCopyConstructor()
{
	SimpleVector<int> numbers(10);
	std::iota(numbers.begin(), numbers.end(), 1);

	SimpleVector<int> dest = numbers;

	ASSERT_EQUAL(dest.Size(), numbers.Size());
	ASSERT(dest.Capacity() >= dest.Size());
	ASSERT(std::equal(dest.begin(), dest.end(), numbers.begin()));
}


void TestCopyAssignment() 
{
	SimpleVector<int> numbers(10);
	std::iota(numbers.begin(), numbers.end(), 1);

	SimpleVector<int> dest;
	ASSERT_EQUAL(dest.Size(), 0u);

	dest = numbers;
	ASSERT_EQUAL(dest.Size(), numbers.Size());
	ASSERT(dest.Capacity() >= dest.Size());
	ASSERT(std::equal(dest.begin(), dest.end(), numbers.begin()));
}


void TestMoveConstructor()
{
	SimpleVector<int> numbers(10);
	std::iota(numbers.begin(), numbers.end(), 1);
	SimpleVector<int> expected = numbers;

	SimpleVector<int> dest = std::move(numbers);

	ASSERT_EQUAL(dest.Size(), expected.Size());
	ASSERT(dest.Capacity() >= dest.Size());
	ASSERT(std::equal(dest.begin(), dest.end(), expected.begin()));

    ASSERT_EQUAL(numbers.Size(), 0);
	ASSERT_EQUAL(numbers.Capacity(), 0);
}


void TestMoveAssignment()
{
	SimpleVector<int> numbers(10);
	std::iota(numbers.begin(), numbers.end(), 1);
	SimpleVector<int> expected = numbers;

	SimpleVector<int> dest;
	ASSERT_EQUAL(dest.Size(), 0u);

	dest = std::move(numbers);

	ASSERT_EQUAL(dest.Size(), expected.Size());
	ASSERT(dest.Capacity() >= dest.Size());
	ASSERT(std::equal(dest.begin(), dest.end(), expected.begin()));

	ASSERT_EQUAL(numbers.Size(), 0);
	ASSERT_EQUAL(numbers.Capacity(), 0);
}


void TestNoCopy() 
{
	SimpleVector<StringNonCopyable> strings;
	static const int SIZE = 10;
	
    for (int i = 0; i < SIZE; ++i)
	{
		strings.PushBack(StringNonCopyable(std::to_string(i)));
	}
	
    for (int i = 0; i < SIZE; ++i) 
	{
		ASSERT_EQUAL(strings[i], std::to_string(i));
	}
}


void TestSimpleVector()
{
    TestRunner tr;

    RUN_TEST(tr, TestConstruction);
    RUN_TEST(tr, TestPushBack);
	RUN_TEST(tr, TestCopyConstructor);
	RUN_TEST(tr, TestCopyAssignment);
	RUN_TEST(tr, TestMoveConstructor);
	RUN_TEST(tr, TestMoveAssignment);
	RUN_TEST(tr, TestNoCopy);
}