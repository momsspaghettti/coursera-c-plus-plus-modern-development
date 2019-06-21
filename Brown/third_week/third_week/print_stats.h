#pragma once
#include <cstdint>
#include <string>
#include <optional>
#include <set>
#include <map>
#include <unordered_map>
#include <istream>
#include <algorithm>


enum class Gender
{
	M,
    W,
};


struct Person
{
	std::string name;
	unsigned age;
	unsigned income;
	Gender gender;
};


std::istream& operator>>(std::istream& input, Person& person);


class PeopleDataBase
{
public:
	PeopleDataBase() = default;

	void InsertNewPerson(const Person& person);

	int GetAdultPeople(const unsigned& year) const;

	uint64_t GetMostWealthyPeopleIncome(size_t count) const;

	std::optional<std::string> GetMostPopularName(const Gender& gender) const;

private:
	std::map<unsigned, int> ages_;
	std::multiset<unsigned> incomes_;
	std::unordered_map<Gender, std::unordered_map<std::string, unsigned>> names_;

	std::optional<std::pair<std::string, unsigned>> most_popular_men_ = std::nullopt;
	std::optional<std::pair<std::string, unsigned>> most_popular_women_ = std::nullopt;

	mutable bool ages_cached_ = false;
	mutable bool incomes_cached_ = false;

	mutable std::map<unsigned, int> ages_cache_;
	mutable std::map<size_t, uint64_t> incomes_cache_;

	void cache_ages() const;
	void cache_incomes() const;
};


template <typename Iterator>
class IteratorRange {
public:
	IteratorRange(Iterator begin, Iterator end)
		: first(begin)
		, last(end)
	{
	}

	Iterator begin() const {
		return first;
	}

	Iterator end() const {
		return last;
	}

private:
	Iterator first, last;
};


template <typename Collection>
auto Head(Collection& v, size_t top) 
{
	return IteratorRange{ v.rbegin(), std::next(v.rbegin(), std::min(top, v.size())) };
}


void PerformQueries(std::istream& input, std::ostream& output);


void TestPrintStats();