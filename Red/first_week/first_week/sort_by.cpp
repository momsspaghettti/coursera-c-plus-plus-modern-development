#include "airline_ticket.h"
#include "UnitTestsFramework.h"
#include <algorithm>
#include <numeric>


#define SORT_BY(field)                                         \
    [](const AirlineTicket& lhs, const AirlineTicket& rhs)     \
    {                                                          \
	    return lhs.field < rhs.field;                          \
    }                                                          


void Test() 
{
    std::vector<AirlineTicket> tixs = {
    {"VKO", "AER", "Utair",     {2018, 2, 28}, {17, 40}, {2018, 2, 28}, {20,  0}, 1200},
    {"AER", "VKO", "Utair",     {2018, 3,  5}, {14, 15}, {2018, 3,  5}, {16, 30}, 1700},
    {"AER", "SVO", "Aeroflot",  {2018, 3,  5}, {18, 30}, {2018, 3,  5}, {20, 30}, 2300},
    {"PMI", "DME", "Iberia",    {2018, 2,  8}, {23, 00}, {2018, 2,  9}, { 3, 30}, 9000},
    {"CDG", "SVO", "AirFrance", {2018, 3,  1}, {13, 00}, {2018, 3,  1}, {17, 30}, 8000},
    };

    std::sort(std::begin(tixs), std::end(tixs), SORT_BY(price));
    ASSERT_EQUAL(tixs.front().price, 1200);
    ASSERT_EQUAL(tixs.back().price, 9000);

    std::sort(std::begin(tixs), std::end(tixs), SORT_BY(from));
    ASSERT_EQUAL(tixs.front().from, "AER");
    ASSERT_EQUAL(tixs.back().from, "VKO");

    std::sort(begin(tixs), end(tixs), SORT_BY(arrival_date));
    ASSERT_EQUAL(tixs.front().arrival_date, (Date{2018, 2, 9}));
    ASSERT_EQUAL(tixs.back().arrival_date, (Date{2018, 3, 5}));
}


void TestSortBy()
{
	TestRunner tr;
	RUN_TEST(tr, Test);
}


bool operator<(const Time& left, const Time& right)
{
	const unsigned total1 = left.hours * 60 + left.minutes;
	const unsigned total2 = right.hours * 60 + right.minutes;

	return total1 < total2;
}


bool operator<(const Date& left, const Date& right)
{
	const unsigned total1 = left.year * 372 + left.month * 31 + left.day;
	const unsigned total2 = right.year * 372 + right.month * 31 + right.day;

	return total1 < total2;
}


bool operator!=(const Time& left, const Time& right)
{
	const unsigned total1 = left.hours * 60 + left.minutes;
	const unsigned total2 = right.hours * 60 + right.minutes;

	return total1 != total2;
}


bool operator!=(const Date& left, const Date& right)
{
	const unsigned total1 = left.year * 372 + left.month * 31 + left.day;
	const unsigned total2 = right.year * 372 + right.month * 31 + right.day;

	return total1 != total2;
}


std::ostream& operator<<(std::ostream& os, const Time& time)
{
	os << time.hours << ":" << time.minutes;
	return os;
}


std::ostream& operator<<(std::ostream& os, const Date& date)
{
	os << date.year << "-" << date.month << "-" << date.day;
	return os;
}