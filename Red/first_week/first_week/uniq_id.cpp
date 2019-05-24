#include <string>
#include <vector>
#include "UnitTestsFramework.h"


#define get_num(line) var##_##line


#define name(line) get_num(line)


#define UNIQ_ID name(__LINE__)


void UniqIdTests()
{
    int UNIQ_ID = 0;
    std::string UNIQ_ID = "hello";
    std::vector<std::string> UNIQ_ID = {"hello", "world"};
    std::vector<int> UNIQ_ID = {1, 2, 3, 4};

	std::vector<std::string> expected_1= { "hello", "world" };
	std::vector<int> expected_2 = { 1, 2, 3, 4 };

	ASSERT_EQUAL(var_17, 0);
	ASSERT_EQUAL(var_18, "hello");
	ASSERT_EQUAL(var_19, expected_1);
	ASSERT_EQUAL(var_20, expected_2);
}


void TestUniqId()
{
	TestRunner tr;

	RUN_TEST(tr, UniqIdTests);
}