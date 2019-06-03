#pragma once
#include <map>
#include <string>


struct Stats
{
	std::map<std::string, int> word_frequences;

	void operator+= (const Stats& other);
};


inline void Stats::operator+=(const Stats& other)
{
	for (const auto& pair_ : other.word_frequences)
	{
		word_frequences[pair_.first] += pair_.second;
	}
}


void TestExploreKeyWords();