#pragma once
#include <vector>
#include "date.h"
#include <string>
#include <list>
#include <algorithm>
#include <map>
#include <unordered_set>


using namespace std;


class Database
{
public:
	Database() = default;

	void Add(const Date& date, const string& event);

	void Print(ostream& out) const;

    template<typename Predicate>
	int RemoveIf(const Predicate& predicate)
	{
		int result = 0;

		map<Date, vector<string>> new_storage;
		map<Date, unordered_set<string>> new_checker;

        for (auto& pair_ : storage_)
        {
            const auto border = stable_partition(pair_.second.begin(), pair_.second.end(), 
				[predicate, pair_](const auto& item)
            {
				return predicate(pair_.first, item);
			});

			const size_t tmp = pair_.second.size();

            if (border == pair_.second.end())
            {
				result += tmp;
            }
            else
            {
				new_storage[pair_.first] = vector<string>(border, pair_.second.end());
				new_checker[pair_.first] = unordered_set<string>(border, pair_.second.end());

				result += tmp - new_storage.at(pair_.first).size();
            }
        }

		storage_ = new_storage;
		checker_ = new_checker;

		return result;
    }

    template<typename Predicate>
	vector<pair<Date, string>> FindIf(const Predicate& predicate) const
    {
		map<Date, vector<string>> tmp;
		vector<pair<Date, string>> result;

        for (const auto& pair_ : storage_)
        {
			vector<string> tmp_vec;

            copy_if(pair_.second.begin(), pair_.second.end(), back_inserter(tmp_vec),
				[predicate, pair_](const auto& item)
			{
				return predicate(pair_.first, item);
			});

            if (tmp_vec.size() != 0)
            {
				tmp[pair_.first] = tmp_vec;
            }
        }

		for (const auto& pair_ : tmp)
		{
		    for (const string& item : pair_.second)
		    {
				result.push_back(make_pair(pair_.first, item));
		    }
		}

		return result;
    }

	pair<Date, string> Last(const Date& date) const;

private:
	map<Date, vector<string>> storage_;
	map<Date, unordered_set<string>> checker_;
};


ostream& operator<<(ostream& os, const pair<Date, vector<string>>& pair_);

bool operator<(const pair<Date, string>& left, const pair<Date, string>& right);

bool operator==(const pair<Date, string>& left, const pair<Date, string>& right);

ostream& operator<<(ostream& os, const pair<Date, string>& pair_);