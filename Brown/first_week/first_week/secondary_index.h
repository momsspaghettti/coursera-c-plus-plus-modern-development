#pragma once
#include <unordered_map>
#include <string>
#include <map>
#include <list>


struct Record
{
	std::string id;
	std::string title;
	std::string user;
	int timestamp;
	int karma;
};


// Реализуйте этот класс
class Database 
{
public:
	bool Put(const Record& record);

	const Record* GetById(const std::string& id) const;

	bool Erase(const std::string& id);

	template <typename Callback>
	void RangeByTimestamp(int low, int high, Callback callback) const;

	template <typename Callback>
	void RangeByKarma(int low, int high, Callback callback) const;

	template <typename Callback>
	void AllByUser(const std::string& user, Callback callback) const;

private:
	using ListIter = std::list<Record>::iterator;

    struct Iterators
    {
		std::multimap<int, Database::ListIter>::iterator timestamp_iterator;
		std::multimap<int, Database::ListIter>::iterator karma_iterator;
		std::multimap<std::string, Database::ListIter>::iterator user_iterator;
    };

	std::list<Record> storage_;
	std::unordered_map<std::string, ListIter> id_to_record_;
	std::multimap<int, ListIter> timestamp_map_;
	std::multimap<int, ListIter> karma_map_;
	std::multimap<std::string, ListIter> user_map_;
	std::unordered_map<std::string, Iterators> id_to_iterators_;

	void inserter(const Record& record);

	void deleter(const std::unordered_map<std::string, ListIter>::iterator& iter);
};


template <typename Callback>
void Database::RangeByTimestamp(int low, int high, Callback callback) const
{
	auto lower = timestamp_map_.lower_bound(low);
	const auto upper = timestamp_map_.upper_bound(high);

	while (lower != upper && callback(*lower->second))
	{
		++lower;
	}
}


template <typename Callback>
void Database::RangeByKarma(int low, int high, Callback callback) const
{
	auto lower = karma_map_.lower_bound(low);
	const auto upper = karma_map_.upper_bound(high);

	while (lower != upper && callback(*lower->second))
	{
		++lower;
	}
}


template <typename Callback>
void Database::AllByUser(const std::string& user, Callback callback) const
{
	auto [first, end] = user_map_.equal_range(user);

    while (first != end && callback(*first->second))
    {
		++first;
    }
}


void TestSecondaryIndex();