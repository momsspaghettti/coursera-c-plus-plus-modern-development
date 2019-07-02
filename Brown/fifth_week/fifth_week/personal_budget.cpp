#include "personal_budget.h"
#include <sstream>


void PersonalBudgetManager::Earn(const Date& from, const Date& to, const double& money)
{
	const double money_per_day = money / 
		(static_cast<double>(Date::ComputeDaysDiff(from, to)) + 1);

	const size_t n = Date::GetIndexFromDay(to);

    for (size_t i = Date::GetIndexFromDay(from); i <= n; ++i)
    {
		money_calendar_[i].earned += money_per_day;
    }
}


void PersonalBudgetManager::PayTax(const class Date& from, const class Date& to, int tax)
{
	if (tax_ != tax)
		tax_ = tax;

	const size_t n = Date::GetIndexFromDay(to);

	for (size_t i = Date::GetIndexFromDay(from); i <= n; ++i)
	{
		money_calendar_[i].earned *= ((100.0 - tax_ * 1.0) / 100.0);
	}
}


void PersonalBudgetManager::Spend(const Date& from, const Date& to, const double& money)
{
	const double money_per_day = money /
		(static_cast<double>(Date::ComputeDaysDiff(from, to)) + 1);

	const size_t n = Date::GetIndexFromDay(to);

	for (size_t i = Date::GetIndexFromDay(from); i <= n; ++i)
	{
		money_calendar_[i].spent += money_per_day;
	}
}


double PersonalBudgetManager::ComputeIncome(const class Date& from, const class Date& to) const
{
	double result = 0;

	const size_t n = Date::GetIndexFromDay(to);

	for (size_t i = Date::GetIndexFromDay(from); i <= n; ++i)
	{
		const auto& money_per_day = money_calendar_[i];

		result += (money_per_day.earned - money_per_day.spent);
	}

	return result;
}


void PerformBudgetQueries(std::istream& input, std::ostream& output)
{
	//output.precision(25);
	input.tie(nullptr);
	std::ios_base::sync_with_stdio(false);

	PersonalBudgetManager budget_manager;

	int n;
	input >> n;

	std::string query;
	Date from, to;

    for (int i = 0; i < n; ++i)
    {
		input >> query >> from >> to;

        if (query == "ComputeIncome")
			output << budget_manager.ComputeIncome(from, to) << '\n';

        if (query == "Earn")
        {
			double value;
			input >> value;

			budget_manager.Earn(from, to, value);
        }

		if (query == "PayTax")
		{
			int tax;
			input >> tax;

			budget_manager.PayTax(from, to, tax);
		}

        if (query == "Spend")
        {
			double value;
			input >> value;

			budget_manager.Spend(from, to, value);
        }
    }
}


Date& Date::FromString(const std::string& str_date)
{
	std::stringstream buff;

    for (const char& ch : str_date)
    {
		ch == '-' ? buff << ' ' : buff << ch;
    }

	buff >> year_ >> month_ >> day_;

	return *this;
}


time_t Date::AsTimeStamp() const
{
	std::tm t;
	t.tm_sec = 0;
	t.tm_min = 0;
	t.tm_hour = 0;
	t.tm_mday = day_;
	t.tm_mon = month_ - 1;
	t.tm_year = year_ - 1900;
	t.tm_isdst = 0;
	return mktime(&t);
}


long long Date::ComputeDaysDiff(const Date& date_from, const Date& date_to)
{
	const time_t timestamp_to = date_to.AsTimeStamp();
	const time_t timestamp_from = date_from.AsTimeStamp();
	const auto SECONDS_IN_DAY = 60 * 60 * 24;
	
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}


size_t Date::GetIndexFromDay(const Date& date)
{
	return static_cast<size_t>(ComputeDaysDiff(Date(2000, 1, 1), date));
}


std::istream& operator>>(std::istream& input, Date& date)
{
	std::string str_date;
	input >> str_date;

	date = Date().FromString(str_date);

	return input;
}


std::ostream& operator<<(std::ostream& input, const Date& date)
{
	input << date.year_ << "-" << date.month_ << "-" <<
		date.day_;

	return input;
}


bool operator!=(const Date& lhs, const Date& rhs)
{
	return lhs.year_ != rhs.year_ ||
		lhs.month_ != rhs.month_ ||
		lhs.day_ != rhs.day_;
}