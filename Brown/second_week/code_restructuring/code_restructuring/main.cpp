#include "UnitTestsFramework.h"
#include "stats_aggregator.h"
#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <functional>


using namespace std;


void TestAll();


unique_ptr<StatsAggregator> ReadAggregators(istream& input) 
{
	using namespace StatsAggregators;

	const unordered_map<string, std::function<unique_ptr<StatsAggregator>()>> known_builders = {
	    {"sum", [] { return make_unique<Sum>(); }},
	    {"min", [] { return make_unique<Min>(); }},
	    {"max", [] { return make_unique<Max>(); }},
	    {"avg", [] { return make_unique<Average>(); }},
	    {"mode", [] { return make_unique<Mode>(); }}
	};

	auto result = make_unique<Composite>();

	int aggr_count;
	input >> aggr_count;

	string line;
	for (int i = 0; i < aggr_count; ++i)
	{
		input >> line;
		result->Add(known_builders.at(line)());
	}

	return result;
}


int main()
{
	TestAll();

	auto stats_aggregator = ReadAggregators(cin);

	for (int value; cin >> value; )
	{
		stats_aggregator->Process(value);
	}

	stats_aggregator->PrintValue(cout);

	return 0;
}


void TestAll()
{
	using namespace StatsAggregators;

	TestRunner tr;
	RUN_TEST(tr, TestSum);
	RUN_TEST(tr, TestMin);
	RUN_TEST(tr, TestMax);
	RUN_TEST(tr, TestAverage);
	RUN_TEST(tr, TestMode);
	RUN_TEST(tr, TestComposite);
}