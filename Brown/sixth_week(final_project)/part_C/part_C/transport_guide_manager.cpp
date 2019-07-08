#include "transport_guide_manager.h"
#include <sstream>
#include <string>
#include <string_view>


void TransportGuideManager::PerformQueries(std::istream& input, 
	std::ostream& output)
{
	output.precision(6);

	int n;
	input >> n;
	input.ignore(1);

	perform_read_queries(n, input);

	routes_database_.BuildAllRoutes(stops_database_);

	input >> n;
	input.ignore(1);

	perform_write_queries(n, input, output);
}


void TransportGuideManager::perform_read_queries(int queries_count, std::istream& input)
{	
    std::string line, query;
	for (int i = 0; i < queries_count; ++i)
	{
		std::getline(input, line, '\n');

		std::stringstream buffer(line);

		buffer >> query;
		buffer.ignore(1);

		if (query == "Stop")
		{
			BusStop stop;
			buffer >> stop;
			AddStop(stop);
		}

		if (query == "Bus")
		{
			std::string route_description;
			std::getline(buffer, route_description, '\n');

			AddRoute(route_description);
		}
	}
}


void TransportGuideManager::perform_write_queries(int queries_count, 
	std::istream& input, std::ostream& output) const
{
	std::string line, query, bus, stop;
	for (int i = 0; i < queries_count; ++i)
	{
		std::getline(input, line, '\n');

		std::stringstream buffer(line);

		buffer >> query;
		buffer.ignore(1);

		if (query == "Bus")
		{
			std::getline(buffer, bus, '\n');

			WriteResponse(ResponseType::BUS, bus, output);
		}

        if (query == "Stop")
        {
			std::getline(buffer, stop, '\n');

			WriteResponse(ResponseType::STOP, stop, output);
        }
	}
}


void TransportGuideManager::AddStop(const BusStop& stop)
{
	stops_database_.AddStop(stop);
}


void TransportGuideManager::AddRoute(const std::string& route_description)
{
	routes_database_.ReadRouteFromString(route_description);
}


void TransportGuideManager::WriteResponse(ResponseType response_type, 
	const std::string& response_param, std::ostream& output) const
{
    switch (response_type)
    {
	case ResponseType::BUS:
		output << routes_database_.GetRouteStats(response_param) << '\n';
        break;

	case ResponseType::STOP:
		output << stops_database_.GetBusStopStat(response_param) << '\n';
        break;
    }
}