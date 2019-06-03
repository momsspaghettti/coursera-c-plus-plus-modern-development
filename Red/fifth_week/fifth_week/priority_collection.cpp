#include "UnitTestsFramework.h"
#include <iterator>
#include <memory>
#include "priority_collection.h"


class StringNonCopyable : public std::string 
{
public:
	using std::string::string;  // ѕозвол€ет использовать конструкторы строки
	StringNonCopyable(const StringNonCopyable&) = delete;
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};


void TestWithoutCopy() 
{
	PriorityCollection<StringNonCopyable> strings;
	const auto white_id = strings.Add("white");
	const auto yellow_id = strings.Add("yellow");
	const auto red_id = strings.Add("red");

	strings.Promote(yellow_id);
	for (int i = 0; i < 2; ++i) 
	{
		strings.Promote(red_id);
	}
	strings.Promote(yellow_id);
	
    {
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
		ASSERT(!strings.IsValid(red_id));
	}
	
    {
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "yellow");
		ASSERT_EQUAL(item.second, 2);
		ASSERT(!strings.IsValid(yellow_id));
	}
	
    {
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "white");
		ASSERT_EQUAL(item.second, 0);
		ASSERT(!strings.IsValid(white_id));
	}

	{
		PriorityCollection<StringNonCopyable> strings;
		std::vector<StringNonCopyable> v;
		v.push_back("one");
		v.push_back("two");
		v.push_back("three");

		std::vector<int> out(3);
		strings.Add(v.begin(), v.end(), out.begin());

        for (int i = 0; i < 3; ++i)
        {
			ASSERT_EQUAL(out[i], i);
			ASSERT(strings.IsValid(out[i]));
        }
	}
}


void TestPriorityCollection() 
{
	TestRunner tr;
	RUN_TEST(tr, TestWithoutCopy);
}
