#include "database.h"
#include <algorithm>
#include <iostream>


void Database::Add(const Date& date, const string& event)
{
	const uint64_t count = checker_[date].size();

	checker_[date].insert(event);

    if (checker_.at(date).size() > count)
    {
		storage_[date].push_back(event);
    }
}


void Database::Print(std::ostream& out) const
{
    for (const auto& pair_ : storage_)
    {
        for (const auto& item : pair_.second)
        {
			out << pair_.first << " " << item << endl;
        }
    }
}


pair<Date, string> Database::Last(const Date& date) const
{
	if (storage_.empty())
		throw invalid_argument("Empty storage");

	auto upper = storage_.upper_bound(date);

    if (upper == storage_.begin())
		throw invalid_argument("Less date");

	--upper;

	return make_pair(upper->first, upper->second.back());
}


ostream& operator<<(ostream& os, const pair<Date, vector<string>>& pair_)
{
	for (const auto& item : pair_.second)
	{
		os << pair_.first << " " << item << endl;
	}

	return os;
}


bool operator<(const pair<Date, string>& left, const pair<Date, string>& right)
{
	return left.first < right.first;
}


bool operator==(const pair<Date, string>& left, const pair<Date, string>& right)
{
	return left.first == right.first && right.second == left.second;
}


ostream& operator<<(ostream& os, const pair<Date, string>& pair_)
{
	os << pair_.first << " " << pair_.second;

	return os;
}