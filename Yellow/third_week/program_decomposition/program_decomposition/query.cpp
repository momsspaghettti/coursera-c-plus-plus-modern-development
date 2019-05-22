#include "query.h"


std::istream& operator >> (std::istream& is, Query& q) 
{
    std::string typeStr;
	is >> typeStr;
	if (typeStr == "NEW_BUS") {
		q.type = QueryType::NewBus;
		is >> q.bus;
		int stop_count = 0;
		is >> stop_count;
		q.stops.resize(stop_count);
		for (auto& stop : q.stops) {
			is >> stop;
		}
	}
	else if (typeStr == "BUSES_FOR_STOP") {
		q.type = QueryType::BusesForStop;
		is >> q.stop;
	}
	else if (typeStr == "STOPS_FOR_BUS") {
		q.type = QueryType::StopsForBus;
		is >> q.bus;
	}
	else if (typeStr == "ALL_BUSES") {
		q.type = QueryType::AllBuses;
	}

	return is;
}