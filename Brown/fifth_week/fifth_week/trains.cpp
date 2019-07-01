#include "trains.h"
#include "UnitTestsFramework.h"
#include <algorithm>


void TrainsManager::Add(int start, int finish)
{
	routes_[start].insert(finish);
	routes_[finish].insert(start);
}


int TrainsManager::Go(int start, int finish) const
{
	const auto start_routes = routes_.find(start);

	if (start_routes == routes_.end())
		return std::abs(start - finish);

	auto lower = start_routes->second.lower_bound(finish);

	int answer = std::abs(start - finish);

	if (lower != start_routes->second.end())
		answer = std::min(answer, 
			std::abs(*lower - finish));

	if (lower != start_routes->second.begin())
		answer = std::min(answer,
			std::abs(*--lower - finish));

	return answer;
}


void PerformTrainsQueries(std::istream& input, std::ostream& output)
{
	input.tie(nullptr);
	std::ios_base::sync_with_stdio(false);

	TrainsManager trains_manager;

	int n;
	input >> n;

	std::string query;
	int start, finish;

	for (int i = 0; i < n; ++i)
	{
		input >> query;

		if (query == "ADD")
		{
			input >> start >> finish;

			trains_manager.Add(start, finish);
		}

		if (query == "GO")
		{
			input >> start >> finish;

			output << trains_manager.Go(start, finish) << '\n';
		}
	}
}


void TestTrainsManager()
{
	TrainsManager trains_manager;

	trains_manager.Add(-2, 5);
	trains_manager.Add(10, 4);
	trains_manager.Add(5, 8);
	ASSERT_EQUAL(trains_manager.Go(4, 10), 0);
	ASSERT_EQUAL(trains_manager.Go(4, -2), 6);
	ASSERT_EQUAL(trains_manager.Go(5, 0), 2);
	ASSERT_EQUAL(trains_manager.Go(5, 100), 92);
	ASSERT_EQUAL(trains_manager.Go(-2, -2), 0);
}


void TestTrainsQueries()
{
	std::string queries =
		"7\n"
		"ADD -2 5\n"
		"ADD 10 4\n"
		"ADD 5 8\n"
		"GO 4 10\n"
		"GO 4 -2\n"
		"GO 5 0\n"
		"GO 5 100\n";

	std::istringstream input(queries);

	std::string expected =
		"0\n"
		"6\n"
		"2\n"
		"92\n";

	std::ostringstream output;

	PerformTrainsQueries(input, output);
	ASSERT_EQUAL(output.str(), expected);
}


void TestTrains()
{
	TestRunner tr;
	RUN_TEST(tr, TestTrainsManager);
	RUN_TEST(tr, TestTrainsQueries);
}