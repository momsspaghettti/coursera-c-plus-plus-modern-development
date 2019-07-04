#include "routes_database.h"


void RoutesDataBase::BuildAllRoutes(const BusStopsDataBase& stops_database)
{
	for (auto& pair_ : routes_)
	{
		pair_.second->Build(stops_database);
	}
}


RoutesDataBase::RouteResponse RoutesDataBase::GetRouteStats(const std::string& bus) const
{
	RouteResponse response;
	response.first = bus;

	const auto finder = routes_.find(bus);

    if (finder != routes_.end())
    {
		response.second = finder->second->GetRouteStats();
    }

	return response;
}


std::ostream& operator<<(std::ostream& output, const RoutesDataBase::RouteResponse& response)
{
	output.precision(6);

	output << "Bus " << response.first << ": ";

    if (response.second)
    {
		output << response.second->stops_on_route << " stops on route, " <<
			response.second->unique_stops << " unique stops, " <<
			response.second->route_length << " route length";
    }
    else
    {
		output << "not found";
    }

	return output;
}


void RoutesDataBase::ReadRouteFromString(std::string_view route_line)
{
	auto pos = route_line.find_first_of(':');
	std::string bus_name = std::string(route_line.substr(0, pos));

	route_line.remove_prefix(pos + 2);	

	const char delimiter =
		route_line.find_first_of('>') != route_line.npos ? '>' : '-';

	std::shared_ptr<IRouteInfo> route_info;

    if (delimiter == '-')
    {
		route_info = std::make_shared<DirectRoute>();
    }
    else
    {
		route_info = std::make_shared<RoundRoute>();
    }

	routes_[bus_name] = route_info;

    while (pos != route_line.npos)
    {
		pos = route_line.find_first_of(delimiter);

		pos = pos == route_line.npos ? pos : pos - 1;
		route_info->AddStop(std::string(route_line.substr(0, pos )));

		route_line.remove_prefix(pos + 3);
    }
}


void IRouteInfo::AddStop(const std::string& stop)
{
	stops_.push_back(stop);
	unique_stops_.insert(stop);
}


const RouteStats& IRouteInfo::GetRouteStats() const
{
	return route_stats_;
}


void DirectRoute::RecomputeStatsInChildClass()
{
	route_stats_.route_length *= 2;
	route_stats_.stops_on_route = 2 * stops_.size() - 1;
}


void RoundRoute::RecomputeStatsInChildClass()
{
	route_stats_.stops_on_route = stops_.size();
}


void IRouteInfo::Build(const BusStopsDataBase& stops_database)
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

		RecomputeStatsInChildClass();

		route_stats_.unique_stops = unique_stops_.size();
    }
}