#include "UnitTestsFramework.h"
#include "group_heavy_strings.h"
#include <algorithm>
#include <string>
#include <vector>


void TestGroupingABC() {
    std::vector<std::string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 2);
    std::sort(begin(groups), end(groups));  // Порядок групп не имеет значения
    
    ASSERT_EQUAL(groups[0], std::vector<std::string>({"caab", "abc", "bacc"}));
    ASSERT_EQUAL(groups[1], std::vector<std::string>({"cccc", "c"}));
}


void TestGroupingReal() 
{
    std::vector<std::string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 4);
    std::sort(begin(groups), end(groups));  // Порядок групп не имеет значения
    
    ASSERT_EQUAL(groups[0], std::vector<std::string>({"laptop", "paloalto"}));
    ASSERT_EQUAL(groups[1], std::vector<std::string>({"law", "wall", "awl"}));
    ASSERT_EQUAL(groups[2], std::vector<std::string>({"port"}));
    ASSERT_EQUAL(groups[3], std::vector<std::string>({"top", "pot"}));
}


void TestVectorsStrings()
{
	std::vector<std::vector<std::string>> strings = { {"lll", "aaa", "www"}, {"ppp", "ooo", "rrr", "ttt"},
		{"ttt", "ooo", "ppp"}, {"lll", "aaa", "ppp", "ttt", "ooo", "ppp"}, {"ppp", "ooo", "ttt"}, 
	    {"ppp", "aaa", "lll", "ooo", "aaa", "lll", "ttt", "ooo"}, {"www", "aaa", "lll", "lll"}, {"aaa", "www", "lll"} };

	auto groups = GroupHeavyStrings(strings);

	ASSERT_EQUAL(groups.size(), 4);

	std::sort(groups.begin(), groups.end());

	ASSERT_EQUAL(groups[0], std::vector<std::vector<std::string>>({ {"lll", "aaa", "ppp", "ttt", "ooo", "ppp"}, 
		{"ppp", "aaa", "lll", "ooo", "aaa", "lll", "ttt", "ooo"} }));
	
    ASSERT_EQUAL(groups[1], std::vector<std::vector<std::string>>({ {"lll", "aaa", "www"}, 
		{"www", "aaa", "lll", "lll"}, {"aaa", "www", "lll"} }));
	
    ASSERT_EQUAL(groups[2], std::vector<std::vector<std::string>>({ {"ppp", "ooo", "rrr", "ttt"} }));
	
    ASSERT_EQUAL(groups[3], std::vector<std::vector<std::string>>({ {"ttt", "ooo", "ppp"}, {"ppp", "ooo", "ttt"} }));
}


void TestGroupHeavyStrings()
{
    TestRunner tr;
    RUN_TEST(tr, TestGroupingABC);
    RUN_TEST(tr, TestGroupingReal);
	RUN_TEST(tr, TestVectorsStrings);
}