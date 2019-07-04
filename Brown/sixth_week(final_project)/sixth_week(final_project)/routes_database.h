#pragma once
#include "stops_database.h"
#include <vector>
#include <optional>
#include <utility>
#include <unordered_set>


struct RouteStats
{
	RouteStats()
	{
		stops_on_route = unique_stops = 0;
		route_length = 0;
	}

	int stops_on_route;
	int unique_stops;
	double route_length;
};


class RouteInfo
{
public:
    RouteInfo()
    {
		stops_.reserve(100);
		route_stats_ = RouteStats();
    }

	void AddStop(const std::string& stop);

	void Build(const BusStopsDataBase& stops_database);

	[[nodiscard]] const RouteStats& GetRouteStats() const;

private:
	RouteStats route_stats_;
	std::vector<std::string> stops_;
	std::unordered_set<std::string> unique_stops_;

    friend void TestReadRouteFromString();
};


class RoutesDataBase
{
public:
    RoutesDataBase()
    {
		routes_.reserve(2000);
    }

	void ReadRouteFromString(std::string_view route_line);

	using RouteResponse = std::pair<std::string, 
    std::optional<RouteStats>>;

	void BuildAllRoutes(const BusStopsDataBase& stops_database);

	[[nodiscard]] RouteResponse GetRouteStats(const std::string& bus) const;

	friend std::ostream& operator<<(std::ostream& output, const RouteResponse& response);

private:
	std::unordered_map<std::string, RouteInfo> routes_;

	friend void TestReadRouteFromString();
};


std::ostream& operator<<(std::ostream& output, 
	const RoutesDataBase::RouteResponse& response);