#pragma once
#include <string>
#include <vector>
#include <iostream>


enum class QueryType 
{
	NewBus,
	BusesForStop,
	StopsForBus,
	AllBuses
};


struct Query 
{
	QueryType type;
	std::string bus;
	std::string stop;
	std::vector<std::string> stops;
};


std::istream& operator >> (std::istream& is, Query& q);