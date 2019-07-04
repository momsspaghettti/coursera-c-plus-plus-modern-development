#pragma once
#include "routes_database.h"
#include "stops_database.h"


class TransportGuideManager
{
public:
    TransportGuideManager()
    {
		stops_database_ = BusStopsDataBase();
		routes_database_ = RoutesDataBase();
    }

	void PerformQueries(std::istream& input = std::cin, 
		std::ostream& output = std::cout);

	void AddStop(const BusStop& stop);

	void AddRoute(const std::string& route_description);

	void WriteResponse(const std::string& bus, std::ostream& output) const;

private:
	BusStopsDataBase stops_database_;
	RoutesDataBase routes_database_;

	void perform_read_queries(int queries_count, std::istream& input);

    void perform_write_queries(int queries_count, 
		std::istream& input, std::ostream& output) const;
};
