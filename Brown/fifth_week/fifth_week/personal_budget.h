#pragma once
#include <string>
#include <iostream>
#include <ctime>
#include <vector>


class Date
{
public:
	Date() = default;

    Date(int year, int month, int day) : year_(year), month_(month), day_(day)
	{}

	Date& FromString(const std::string& str_date);

	[[nodiscard]] time_t AsTimeStamp() const;

	static long long ComputeDaysDiff(const Date& date_from, const Date& date_to);

	static size_t GetIndexFromDay(const Date& date);

private:
	int year_, month_, day_;

	friend void TestReadDate();

    friend std::ostream& operator<<(std::ostream& input, const Date& date);

	friend bool operator !=(const Date& lhs, const Date& rhs);
};


std::istream& operator>>(std::istream& input, Date& date);


std::ostream& operator<<(std::ostream& input, const Date& date);


class PersonalBudgetManager
{
public:
	PersonalBudgetManager()
	{
		money_calendar_ = std::vector<MoneyPerDay>(36600, MoneyPerDay());
	}

	void Earn(const Date& from, const Date& to, const double& money);

	void PayTax(const Date& from, const Date& to, int tax=13);

	void Spend(const Date& from, const Date& to, const double& money);

	[[nodiscard]] double ComputeIncome(const Date& from, const Date& to) const;

private:
    struct MoneyPerDay
    {
        MoneyPerDay()
        {
			earned = 0;
			spent = 0;
        }

		double earned;
		double spent;
    };

	int tax_ = 13;

	std::vector<MoneyPerDay> money_calendar_;
};


void PerformBudgetQueries(std::istream& input=std::cin, std::ostream& output=std::cout);


void TestPersonalBudget();