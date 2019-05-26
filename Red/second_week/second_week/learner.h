#pragma once
#include <string>
#include <vector>
#include <set>


class Learner
{
public:
	int Learn(const std::vector<std::string>& words);

	std::vector<std::string> KnownWords();

private:
	std::set<std::string> dict_;
};


void TestLearner();