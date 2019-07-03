#include "routes_database.h"


void RoutesDataBase::BuildAllRoutes(const BusStopsDataBase& stops_database)
{
    for (auto& pair_ : routes_)
    {
		pair_.second.Build(stops_database);
    }
}


RoutesDataBase::RouteResponse RoutesDataBase::GetRouteStats(const std::string& bus) const
{
	RouteResponse response;
	response.first = bus;

	const auto finder = routes_.find(bus);

    if (finder != routes_.end())
    {
		response.second = finder->second.GetRouteStats();
    }

	return response;
}


void RoutesDataBase::ReadRouteFromString(std::string_view route_line)
{
	auto pos = route_line.find_first_of(": ");

	RouteInfo& route_info = 
		routes_[std::string(route_line.substr(0, pos))];
	route_line.remove_prefix(pos + 2);

	const char delimiter =
		route_line.find_first_of('>') != route_line.npos ? '>' : '-';

    while (pos != route_line.npos)
    {
		pos = route_line.find_first_of(delimiter);

		pos = pos == route_line.npos ? pos : pos - 1;
		route_info.AddStop(std::string(route_line.substr(0, pos )));

		route_line.remove_prefix(pos + 3);
    }
}


void RouteInfo::AddStop(const std::string& stop)
{
	stops_.push_back(stop);
}


const RouteStats& RouteInfo::GetRouteStats() const
{
	return route_stats_;
}


void RouteInfo::Build(const BusStopsDataBase& stops_database)
{
    if (stops_.empty())
        return;

    if (stops_.size() == 1)
    {
		route_stats_.stops_on_route = 1;
		route_stats_.unique_stops = 1;
		route_stats_.route_length = 0;
    }
    else
    {
		for (size_t i = 0; i < stops_.size() - 1; ++i)
		{
			route_stats_.route_length += 
				stops_database.ComputeDistanceBetweenStops(stops_[i], stops_[i + 1]);
		}

        if (*stops_.cbegin() == *stops_.crbegin())
        {
			route_stats_.stops_on_route = stops_.size();
			route_stats_.unique_stops = stops_.size() - 1;
        }
        else
        {
			route_stats_.stops_on_route = 2 * stops_.size() - 1;
			route_stats_.unique_stops = stops_.size();
			route_stats_.route_length *= 2;
        }
    }
}