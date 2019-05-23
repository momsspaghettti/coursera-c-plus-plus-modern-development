#pragma once
#include <string>


struct Date 
{
  int year, month, day;
};


struct Time 
{
	int hours;
    int minutes;
};


struct AirlineTicket 
{
    std::string from;
    std::string to;
    std::string airline;
    Date departure_date;
    Time departure_time;
    Date arrival_date;
    Time arrival_time;
    int price;
};


void TestSortBy();
void TestUpdates();


bool operator<(const Time& left, const Time& right);


bool operator<(const Date& left, const Date& right);


bool operator!=(const Time& left, const Time& right);


bool operator!=(const Date& left, const Date& right);


std::ostream& operator<<(std::ostream& os, const Time& time);


std::ostream& operator<<(std::ostream& os, const Date& date);


std::istream& operator>>(std::istream& is, Time& time);


std::istream& operator>>(std::istream& is, Date& date);