#pragma once
#include <unordered_map>
#include <set>
#include <iostream>


class TrainsManager
{
public:
	void Add(int start, int finish);

	int Go(int start, int finish) const;

private:
	std::unordered_map<int, std::set<int>> routes_;
};


void PerformTrainsQueries(std::istream& input, std::ostream& output);


void TestTrains();