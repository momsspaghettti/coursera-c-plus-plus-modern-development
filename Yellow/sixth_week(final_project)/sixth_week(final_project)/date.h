#pragma once
#include <sstream>


using namespace std;


class Date
{
public:
	Date() = default;

	Date(const int& year, const int& month, const int& day);

	int GetYear() const;
	int GetMonth() const;
	int GetDay() const;

	Date& operator=(const Date& other);

private:
	int year_;
	int month_;
    int day_;
};


bool operator<(const Date& left, const Date& right);
bool operator<=(const Date& left, const Date& right);
bool operator>(const Date& left, const Date& right);
bool operator>=(const Date& left, const Date& right);
bool operator==(const Date& left, const Date& right);
bool operator!=(const Date& left, const Date& right);


Date ParseDate(istream& is);

ostream& operator<<(ostream& os, const Date& date);