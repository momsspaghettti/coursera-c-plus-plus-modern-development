#include "date.h"
#include <string>
#include <iomanip>


Date::Date(const int& year, const int& month, 
	const int& day) : year_(year), month_(month), day_(day){}


int Date::GetYear() const
{
	return year_;
}


int Date::GetMonth() const
{
	return month_;
}


int Date::GetDay() const
{
	return day_;
}


bool operator<(const Date& left, const Date& right)
{
	const long total1 = left.GetYear() * 372 + left.GetMonth() * 31 + left.GetDay();
	const long total2 = right.GetYear() * 372 + right.GetMonth() * 31 + right.GetDay();

	return total1 < total2;
}

bool operator<=(const Date& left, const Date& right)
{
	const long total1 = left.GetYear() * 372 + left.GetMonth() * 31 + left.GetDay();
	const long total2 = right.GetYear() * 372 + right.GetMonth() * 31 + right.GetDay();

	return total1 <= total2;
}

bool operator>(const Date& left, const Date& right)
{
	const long total1 = left.GetYear() * 372 + left.GetMonth() * 31 + left.GetDay();
	const long total2 = right.GetYear() * 372 + right.GetMonth() * 31 + right.GetDay();

	return total1 > total2;
}

bool operator>=(const Date& left, const Date& right)
{
	const long total1 = left.GetYear() * 372 + left.GetMonth() * 31 + left.GetDay();
	const long total2 = right.GetYear() * 372 + right.GetMonth() * 31 + right.GetDay();

	return total1 >= total2;
}

bool operator==(const Date& left, const Date& right)
{
	const long total1 = left.GetYear() * 372 + left.GetMonth() * 31 + left.GetDay();
	const long total2 = right.GetYear() * 372 + right.GetMonth() * 31 + right.GetDay();

	return total1 == total2;
}

bool operator!=(const Date& left, const Date& right)
{
	const long total1 = left.GetYear() * 372 + left.GetMonth() * 31 + left.GetDay();
	const long total2 = right.GetYear() * 372 + right.GetMonth() * 31 + right.GetDay();

	return total1 != total2;
}


Date ParseDate(istream& is)
{
	int year, month, day;

	is >> year;
	is.ignore(1);

	is >> month;
	is.ignore(1);

	is >> day;

	return Date(year, month, day);
}


ostream& operator<<(ostream& os, const Date& date)
{
	os << setw(4) << setfill('0') << date.GetYear() << "-" << setw(2) << setfill('0') <<
		date.GetMonth() << "-" << setw(2) << setfill('0') << date.GetDay();

	return os;
}


Date& Date::operator=(const Date& other)
{
	this->year_ = other.GetYear();
	this->month_ = other.GetMonth();
	this->day_ = other.GetDay();

	return *this;
}