#include "routes_database.h"


void RoutesDataBase::BuildAllRoutes(const std::shared_ptr<BusStopsDataBase>& stops_database, 
	const std::shared_ptr<NavigationDataBase>& navigation_database)
{
	for (auto& pair_ : *routes_)
	{
		pair_.second->Build(stops_database, navigation_database, pair_.first);
	}
}


RoutesDataBase::RouteResponse RoutesDataBase::GetRouteStats(const std::string& bus) const
{
	RouteResponse response;
	response.first = bus;

	const auto finder = routes_->find(bus);

    if (finder != routes_->end())
    {
		response.second = finder->second->GetRouteStats();
    }

	return response;
}


void RoutesDataBase::AddRoute(
	const std::string& bus_name, const std::shared_ptr<IRouteInfo>& route_info)
{
    (*routes_)[bus_name] = route_info;
	route_names_.insert(bus_name);
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


bool IRouteInfo::AreStopsNeighbors(const std::string &first, const std::string &second) const {
    for (const auto& first_number : stop_name_to_numbers_->at(first)) {
        for (const auto& second_number : stop_name_to_numbers_->at(second)) {
            if (first_number + 1 == second_number || second_number + 1 == first_number) {
                return true;
            }
        }
    }
    return false;
}


void DirectRoute::RecomputeStatsInChildClass(const std::shared_ptr<BusStopsDataBase>& stops_database, 
	const std::shared_ptr<NavigationDataBase>& navigation_database, const std::string& route_name)
{
    all_stops_ = stops_;
    all_stops_.reserve(stops_.size() - 1);

	for (size_t i = stops_.size() - 1; i > 0; --i)
	{
		const auto real_distance =
			stops_database->ComputeRealDistanceBetweenStops(stops_[i], stops_[i - 1]);

		route_stats_.route_length += real_distance;

		navigation_database->AddReverseStop(route_name, stops_[i], real_distance);

		all_stops_.push_back(stops_[i - 1]);
	}
	navigation_database->AddReverseStop(route_name, *stops_.cbegin(), 0);

	route_stats_.direct_distance *= 2;
	route_stats_.stops_on_route = 2 * stops_.size() - 1;

	size_t index = 0;
	for (const auto& stop : all_stops_) {
        (*stop_name_to_numbers_)[stop].push_back(index);
        ++index;
	}
}


const std::vector<std::string>& DirectRoute::GetStops() const {
    return all_stops_;
}


std::vector<std::string> DirectRoute::GetEndStops() const {
    std::vector<std::string> result;
    result.reserve(2);
    result.push_back(*stops_.cbegin());

    if (*stops_.cbegin() != *stops_.crbegin())
        result.push_back(*stops_.crbegin());

    return result;
}


void RoundRoute::RecomputeStatsInChildClass(const std::shared_ptr<BusStopsDataBase>& stops_database, 
	const std::shared_ptr<NavigationDataBase>& navigation_database, const std::string& route_name)
{
	route_stats_.stops_on_route = stops_.size();

	size_t index = 0;
	for (const auto& stop : stops_) {
        (*stop_name_to_numbers_)[stop].push_back(index);
        ++index;
	}
}


const std::vector<std::string>& RoundRoute::GetStops() const {
    return stops_;
}


std::vector<std::string> RoundRoute::GetEndStops() const {
    std::vector<std::string> result;
    result.reserve(1);
    result.push_back(*stops_.cbegin());

    return result;
}


void IRouteInfo::Build(const std::shared_ptr<BusStopsDataBase>& stops_database, 
	const std::shared_ptr<NavigationDataBase>& navigation_database, const std::string& route_name)
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
            const auto real_distance =
				stops_database->ComputeRealDistanceBetweenStops(stops_[i], stops_[i + 1]);

			route_stats_.route_length += real_distance;

			navigation_database->AddDirectStop(route_name, stops_[i], real_distance);
            
			route_stats_.direct_distance +=
				stops_database->ComputeDirectDistanceBetweenStops(stops_[i], stops_[i + 1]);

			stops_database->AddBusOnStop(route_name, stops_[i]);
		}
		navigation_database->AddDirectStop(route_name, *stops_.crbegin(), 0);

		stops_database->AddBusOnStop(route_name, *stops_.crbegin());

		RecomputeStatsInChildClass(stops_database, navigation_database, route_name);

		route_stats_.unique_stops = unique_stops_.size();

		route_stats_.curvature = route_stats_.route_length / route_stats_.direct_distance;
    }
}