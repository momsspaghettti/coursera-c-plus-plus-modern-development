#include "priority_collection.h"
#include "UnitTestsFramework.h"


void TestNoCopy() 
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
	}
	
    {
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "yellow");
		ASSERT_EQUAL(item.second, 2);
	}
	
    {
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "white");
		ASSERT_EQUAL(item.second, 0);
	}

	{
		PriorityCollection<StringNonCopyable> s;

		std::vector<StringNonCopyable> in;
		in.emplace_back("white");
		in.emplace_back("yellow");
		in.emplace_back("red");
		in.emplace_back("brown");
		std::vector<size_t> out(4);

		s.Add(in.begin(), in.end(), out.begin());
	}
}


void TestPriorityCollection()
{
	TestRunner tr;
	RUN_TEST(tr, TestNoCopy);
}