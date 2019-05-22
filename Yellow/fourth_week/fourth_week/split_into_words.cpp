#include "split_into_words.h"
#include "UnitTestsFramework.h"
#include <algorithm>


void TestSplitIntoWords()
{
    AssertEqual(SplitIntoWords("jdvk kdv kdvk"), std::vector<std::string>{ "jdvk", "kdv", "kdvk" }, "3 words");

    AssertEqual(SplitIntoWords("C Cpp Java Python"), std::vector<std::string>{ "C", "Cpp", "Java", "Python" }, "4 words");

    AssertEqual(SplitIntoWords("Python"), std::vector<std::string>{ "Python" }, "1 words");

	AssertEqual(SplitIntoWords("a b"), std::vector<std::string>{"a", "b"}, "2 words");
}


void TestAll()
{
	TestRunner test_runner;

	test_runner.RunTest(TestSplitIntoWords, "TestSplitIntoWords");
}


std::vector<std::string> SplitIntoWords(const std::string& s)
{
	std::vector<std::string> result;

	auto left_border = std::begin(s);
	auto right_border = std::begin(s);

    while (right_border != s.end())
    {
		left_border = right_border;
		right_border = std::find(left_border, s.end(), ' ');

		result.push_back({ left_border, right_border });

        if (right_border != s.end())
        {
			++right_border;
        }
    }

	return result;
}
