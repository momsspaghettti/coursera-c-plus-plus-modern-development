#include "stops_database.h"
#include <cmath>
#include <sstream>
#include <utility>
#include <stdexcept>


std::istream& operator>>(std::istream& input, GroundPoint& point)
{
	std::string line1;
	std::getline(input, line1, ',');

	std::string line2;
	std::getline(input, line2, ' ');
	std::getline(input, line2, ' ');

	std::istringstream tmp_stream(line1 + " " + line2);
	tmp_stream >> point.latitude_ >> point.longitude_;

	return input;
}


std::ostream& operator<<(std::ostream& output, const GroundPoint& point)
{
	output << point.latitude_ << ", " << point.longitude_;

	return output;
}


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


std::istream& operator>>(std::istream& input, BusStop& bus_stop)
{
	std::string line;
	std::getline(input, line, ':');
	bus_stop.name = std::move(line);

	input.ignore(1);

	input >> bus_stop.coordinate;

	return input;
}


void BusStopsDataBase::AddStop(const BusStop& bus_stop)
{
	bus_stops_[bus_stop.name] = bus_stop.coordinate;
}


double BusStopsDataBase::ComputeDistanceBetweenStops(
	const std::string& from, const std::string& to) const
{
	return GroundPoint::ComputeDistance(
		bus_stops_.at(from), bus_stops_.at(to));
}