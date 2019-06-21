#include "print_stats.h"
#include "UnitTestsFramework.h"
#include <sstream>
#include <iostream>
#include <numeric>


std::istream& operator>>(std::istream& input, Person& person)
{
	std::string gender_;

	input >> person.name >> person.age >> person.income >> gender_;

	if (gender_ == "M")
	{
		person.gender = Gender::M;
	}
	else
	{
		person.gender = Gender::W;
	}

	return input;
}


void PeopleDataBase::InsertNewPerson(const Person& person)
{
	++ages_[person.age];
	incomes_.insert(person.income);
	++names_[person.gender][person.name];

	const unsigned age_ = names_.at(person.gender).at(person.name);

    if (person.gender == Gender::M)
    {
		if (!most_popular_men_)
			most_popular_men_ = { person.name, age_ };
        else
        {
            if (age_ > most_popular_men_->second)
            {
				most_popular_men_ = { person.name, age_ };
            }
            else if (age_ == most_popular_men_->second && person.name < most_popular_men_->first)
            {
				most_popular_men_ = { person.name, age_ };
            }
        }
    }
    else
    {
		if (!most_popular_women_)
			most_popular_women_ = { person.name, age_ };
		else
		{
			if (age_ > most_popular_women_->second)
			{
				most_popular_women_ = { person.name, age_ };
			}
			else if (age_ == most_popular_women_->second && person.name < most_popular_women_->first)
			{
				most_popular_women_ = { person.name, age_ };
			}
		}
    }
}


int PeopleDataBase::GetAdultPeople(const unsigned& year) const
{
	if (!ages_cached_)
	{
		cache_ages();
		ages_cached_ = true;
	}

	const auto lower = ages_cache_.lower_bound(year);

	if (lower == ages_cache_.end())
		return 0;

	return lower->second;
}


uint64_t PeopleDataBase::GetMostWealthyPeopleIncome(size_t count) const
{
	if (!incomes_cached_)
	{
		cache_incomes();
		incomes_cached_ = true;
	}

	const auto lower = incomes_cache_.lower_bound(count);

	if (lower == incomes_cache_.end())
		return incomes_cache_.rbegin()->second;

	return lower->second;
}


std::optional<std::string> PeopleDataBase::GetMostPopularName(const Gender& gender) const
{
    if (gender == Gender::M)
    {
		if (most_popular_men_)
			return most_popular_men_->first;

		return std::nullopt;
    }

	if (most_popular_women_)
		return most_popular_women_->first;

	return std::nullopt;
}


void PeopleDataBase::cache_ages() const
{
	int sum = 0;

    for (auto iter = ages_.rbegin(); iter != ages_.rend(); ++iter)
    {
		sum += iter->second;
		ages_cache_.insert({ iter->first, sum });
    }
}


void PeopleDataBase::cache_incomes() const
{
	int i = 0;
	unsigned sum = 0;

    for (auto iter = incomes_.rbegin(); iter != incomes_.rend(); ++iter)
    {
		sum += *iter;
		++i;

		incomes_cache_.insert({ i, sum });
    }
}


void PerformQueries(std::istream& input, std::ostream& output)
{
	PeopleDataBase pdb;

	int n;

	input >> n;

	Person person;

    for (int i = 0; i < n; ++i)
    {
		input >> person;

		pdb.InsertNewPerson(person);
    }

	std::string command;
	std::string line;
	std::stringstream stream;

	int counter = 0;

    while (std::getline(input, line))
    {
		stream.clear();
		stream << line;

		stream >> command;

        if (command == "AGE")
        {
			if (counter != 0)
				output << '\n';

			++counter;

			unsigned year;
			stream >> year;

			output << "There are " << pdb.GetAdultPeople(year) << " adult people for maturity age " << year;
        }

        if (command == "WEALTHY")
        {
			if (counter != 0)
				output << '\n';

			++counter;

			size_t count;

			stream >> count;

			output << "Top-" << count << " people have total income " << pdb.GetMostWealthyPeopleIncome(count);
        }

        if (command == "POPULAR_NAME")
        {
			if (counter != 0)
				output << '\n';

			++counter;

			std::string g;
			std::optional<std::string> name;

			stream >> g;

            if (g.empty())
                continue;

			g == "M" ? name = pdb.GetMostPopularName(Gender::M) : name = pdb.GetMostPopularName(Gender::W);

			if (name)
			{
				output << "Most popular name among people of gender " << g << " is " << *name;
			}
            else
            {
				output << "No people of gender " << g;
            }
        }
    }
}


void PrintStatsTests()
{
	std::stringstream input;

	input << "11\nIvan 25 1000 M\nOlga 30 623 W\nSergey 24 825 M\n";
	input << "Maria 42 1254 W\nMikhail 15 215 M\nOleg 18 230 M\nDenis 53 8965 M\n";
	input << "Maxim 37 9050 M\nIvan 47 19050 M\nIvan 17 50 M\nOlga 23 550 W\n";
	input << "AGE 18\nAGE 25\nWEALTHY 5\nPOPULAR_NAME M\nWEALTHY 15\nAGE 0";

	std::stringstream output;

	PerformQueries(input, output);

	std::string expected = "There are 9 adult people for maturity age 18\nThere are 6 adult people for maturity age 25\n";
	expected += "Top-5 people have total income 39319\nMost popular name among people of gender M is Ivan\nTop-15 people have total income 41812\n";
	expected += "There are 11 adult people for maturity age 0";

	ASSERT_EQUAL(output.str(), expected);
}


void TestPrintStats()
{
	TestRunner tr;
	RUN_TEST(tr, PrintStatsTests);
}