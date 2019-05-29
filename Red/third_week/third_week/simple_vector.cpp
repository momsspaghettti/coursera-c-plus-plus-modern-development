#include "simple_vector.h"
#include "UnitTestsFramework.h"
#include <algorithm>
#include <vector>
#include <string>


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


void TestSimpleVector()
{
    TestRunner tr;
    
    RUN_TEST(tr, TestConstruction);
    RUN_TEST(tr, TestPushBack);
}