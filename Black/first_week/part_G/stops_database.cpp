#include "stops_database.h"
#include <cmath>
#include <sstream>
#include <utility>
#include <cstdint>


bool operator!=(const GroundPoint& lhs, const GroundPoint& rhs)
{
	return lhs.latitude_ != rhs.latitude_ ||
		lhs.longitude_ != rhs.longitude_;
}


double GroundPoint::ComputeDistance(const GroundPoint& lhs, 
	const GroundPoint& rhs)
{
	constexpr double pi_per_deg = PI / 180;

	const double lat1 = lhs.latitude_ * pi_per_deg;
	const double lat2 = rhs.latitude_ * pi_per_deg;

	const double long1 = lhs.longitude_ * pi_per_deg;
	const double long2 = rhs.longitude_ * pi_per_deg;

	return std::acos(std::sin(lat1) * std::sin(lat2) + 
		std::cos(lat1) * std::cos(lat2) *
		std::cos(std::abs(long1 - long2))) * EARTH_RADIUS;
}


void BusStopStats::AddBusInStat(const std::string& bus)
{
	buses_on_stop_.insert(bus);
}


const std::set<std::string>& BusStopStats::GetStat() const
{
	return buses_on_stop_;
}


const std::unordered_map<std::string, unsigned>& 
BusStopStats::GetDistanceToOtherStops() const
{
	return distance_to_other_stops_;
}


const GroundPoint& BusStopStats::GetCoordinate() const
{
	return coordinate_;
}


void BusStopStats::SetCoordinate(const GroundPoint& coordinate)
{
	coordinate_ = coordinate;
}


void BusStopStats::SetDistanceToOtherStops(
	const std::unordered_map<std::string, unsigned>& other_stops)
{
	distance_to_other_stops_ = other_stops;
}


void BusStopsDataBase::AddStop(const BusStop& bus_stop)
{
	auto bus_stop_stat = std::make_shared<BusStopStats>();
	bus_stop_stat->SetCoordinate(bus_stop.coordinate);
	bus_stop_stat->SetDistanceToOtherStops(bus_stop.distance_to_other_stops);
	bus_stops_[bus_stop.name] = bus_stop_stat;
}


void BusStopsDataBase::AddBusOnStop(const std::string& bus, const std::string& stop)
{
	bus_stops_[stop]->AddBusInStat(bus);
}


BusStopsDataBase::BusStopResponse 
BusStopsDataBase::GetBusStopStat(const std::string& stop) const
{
	BusStopResponse response;
	response.first = stop;

	const auto finder = bus_stops_.find(stop);

	if (finder != bus_stops_.end())
		response.second = finder->second->GetStat();

	return response;
}


size_t BusStopsDataBase::GetBusStopsCount() const
{
	return bus_stops_.size();
}


double BusStopsDataBase::ComputeDirectDistanceBetweenStops(
	const std::string& from, const std::string& to) const
{
	return GroundPoint::ComputeDistance(
		bus_stops_.at(from)->GetCoordinate(), 
		bus_stops_.at(to)->GetCoordinate());
}


unsigned BusStopsDataBase::ComputeRealDistanceBetweenStops(
	const std::string& from, const std::string& to) const
{
	const auto& from_other_stops = bus_stops_.at(from)->GetDistanceToOtherStops();

	const auto from_to_finder = from_other_stops.find(to);

    if (from_to_finder != from_other_stops.end())
    {
		return from_to_finder->second;
    }

	const auto& to_other_stops = bus_stops_.at(to)->GetDistanceToOtherStops();
	const auto to_from_finder = to_other_stops.find(from);

	return to_from_finder->second;
}