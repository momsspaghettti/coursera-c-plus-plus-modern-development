#include "bus_manager.h"


void BusManager::AddBus(const std::string& bus, const std::vector<std::string>& stops)
{
	buses_to_stops.insert(make_pair(bus, stops));
	for (const auto& stop : stops)
	{
		stops_to_buses[stop].push_back(bus);
	}
}


BusesForStopResponse BusManager::GetBusesForStop(const std::string& stop) const
{
	if (stops_to_buses.count(stop) == 0)
	{
		return BusesForStopResponse{ std::vector<std::string>() };
	}
	else
	{
		return BusesForStopResponse{ stops_to_buses.at(stop) };
	}
}


StopsForBusResponse BusManager::GetStopsForBus(const std::string& bus) const
{
	std::vector<std::pair<std::string, std::vector<std::string>>> result;

	if (buses_to_stops.count(bus) > 0)
	{
		for (const auto& stop : buses_to_stops.at(bus))
		{
			result.push_back(std::make_pair(stop, stops_to_buses.at(stop)));
		}
	}

	return StopsForBusResponse{ bus, result };
}


AllBusesResponse BusManager::GetAllBuses() const
{
	return AllBusesResponse{ buses_to_stops };
}
