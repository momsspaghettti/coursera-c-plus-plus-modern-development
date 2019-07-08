#include "stops_database.h"
#include <cmath>
#include <sstream>
#include <utility>
#include <cstdint>


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
	std::getline(input, line);

	std::string_view parser = line;
	auto pos = parser.find_first_of(':');

	if (pos == parser.npos)
	{
		input.ignore(1);
		return input;
	}

	bus_stop.name = parser.substr(0, pos);
	parser.remove_prefix(pos + 2);

	pos = parser.find_first_of(',');
	std::stringstream buffer;
	buffer << std::string(parser.substr(0, pos)) << ", ";
	parser.remove_prefix(pos + 2);
	pos = parser.find_first_of(',');
	buffer << std::string(parser.substr(0, pos));
	buffer >> bus_stop.coordinate;

    if (pos != parser.npos)
    {
		parser.remove_prefix(pos + 2);

        while (pos != parser.npos)
        {
			pos = parser.find_first_of(',');
			bus_stop.distance_to_other_stops.insert(
				ParseDistanceToStop(parser.substr(0, pos)));

			parser.remove_prefix(pos + 2);
        }
    }

	return input;
}


std::pair<std::string, unsigned> ParseDistanceToStop(std::string_view line)
{
	const auto pos = line.find_first_of('m');
	unsigned second = std::stoi(std::string(line.substr(0, pos)));

	line.remove_prefix(pos + 5);

	return std::make_pair(std::string(line.substr(0, 
		line.npos)), second);
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


void BusStopStats::SetDistanceToOtherStops(const std::unordered_map<std::string, unsigned>& other_stops)
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


double BusStopsDataBase::ComputeDirectDistanceBetweenStops(
	const std::string& from, const std::string& to) const
{
	return GroundPoint::ComputeDistance(
		bus_stops_.at(from)->GetCoordinate(), 
		bus_stops_.at(to)->GetCoordinate());
}


uint64_t BusStopsDataBase::ComputeRealDistanceBetweenStops(
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


std::ostream& operator<<(std::ostream& output, const BusStopsDataBase::BusStopResponse& response)
{
	output.precision(6);

	output << "Stop " << response.first << ": ";

    if (response.second)
    {
        if(response.second->empty())
        {
			output << "no buses";
        }
        else
        {
			output << "buses ";

            for (const auto& bus : *response.second)
            {
				output << bus << " ";
            }
        }
    }
    else
    {
		output << "not found";
    }

	return output;
}