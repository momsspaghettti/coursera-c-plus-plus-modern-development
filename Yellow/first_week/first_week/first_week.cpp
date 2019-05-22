#include "pch.h"
#include <iostream>
#include <vector>
#include "Matrix.h"
#include <tuple>
#include <map>
#include <set>
#include <unordered_set>
#include "TeamTasks.h"
#include <sstream>
#include "Templates.h"


using namespace std;


ostream& operator<<(ostream& stream, const vector<unsigned>& vec)
{
	for (const unsigned& item : vec)
	{
		stream << item << " ";
	}

	return stream;
}


void AvgTemperature()
{
	unsigned n;
	cin >> n;

	vector<int> temperatures(n);

	int64_t avg = 0;

	for (int& temperature : temperatures)
	{
		cin >> temperature;

		avg += temperature;
	}

	avg /= n;

	int k = 0;
	vector<unsigned> result;
	unsigned i = 0;

	for (const int& temperature : temperatures)
	{
		if (temperature > avg)
		{
			k += 1;
			result.push_back(i);
		}

		++i;
	}

	cout << k << endl;
	cout << result;
}


void BlocksWeight()
{
	unsigned n;
	uint16_t r;

	cin >> n >> r;

	uint64_t sum = 0;
	uint32_t w, h, d;
	uint64_t tmp_sum = 1;

	for (unsigned i = 0; i < n; ++i)
	{
		cin >> w >> h >> d;

		tmp_sum = 1;

		tmp_sum *= w;
		tmp_sum *= h;
		tmp_sum *= d;

		sum += tmp_sum;
	}

	sum *= r;

	cout << sum;
}


enum class Lang {
	DE, FR, IT
};


struct Region {
	string std_name;
	string parent_std_name;
	map<Lang, string> names;
	int64_t population;
};


bool operator<(const Region& first_region, const Region& second_region)
{
	const auto first_tuple = make_tuple(first_region.std_name, first_region.parent_std_name, first_region.names, first_region.population);
	const auto second_tuple = make_tuple(second_region.std_name, second_region.parent_std_name, second_region.names, second_region.population);

	return first_tuple < second_tuple;
}


int FindMaxRepetitionCount(const vector<Region>& regions)
{
	map<Region, int> result_map;

	for (const Region& region : regions)
	{
		result_map[region] += 1;
	}

	int max = 0;

	for (const auto& pair : result_map)
	{
		if (pair.second > max)
		{
			max = pair.second;
		}
	}

	return max;
}


int main()
{
	map<int, vector<string>> m = { {0, {"sc", "ksv"}} };
	vector<string>& item = GetRefStrict(m, 1);
	item = { "newvalue" };
	cout << m[0] << endl; // выведет newvalue

	return 0;
}
