#pragma once
#include "stops_database.h"
#include <vector>
#include <optional>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cstdint>
#include "navigation_database.h"


struct RouteStats
{
	RouteStats()
	{
		stops_on_route = unique_stops = 0;
		route_length = 0;
	    direct_distance = curvature = 0;
	}

	int stops_on_route;
	int unique_stops;
	uint64_t route_length;
	double direct_distance;
	double curvature;
};


class IRouteInfo
{
public:
    IRouteInfo()
    {
		stops_.reserve(100);
		route_stats_ = RouteStats();
    }

	void AddStop(const std::string& stop);

	void Build(const std::shared_ptr<BusStopsDataBase>& stops_database, 
		const std::shared_ptr<NavigationDataBase>&, const std::string& route_name);

	virtual void RecomputeStatsInChildClass(const std::shared_ptr<BusStopsDataBase>&, 
		const std::shared_ptr<NavigationDataBase>&, const std::string&) = 0;

	[[nodiscard]] const RouteStats& GetRouteStats() const;

	virtual ~IRouteInfo() = default;

private:
	RouteStats route_stats_;
	std::vector<std::string> stops_;
	std::unordered_set<std::string> unique_stops_;

	friend class DirectRoute;
	friend class RoundRoute;
};


class DirectRoute : public IRouteInfo
{
public:
	void RecomputeStatsInChildClass(const std::shared_ptr<BusStopsDataBase>&, 
		const std::shared_ptr<NavigationDataBase>&, const std::string&) override;
};


class RoundRoute : public IRouteInfo
{
public:
    void RecomputeStatsInChildClass(const std::shared_ptr<BusStopsDataBase>&, 
		const std::shared_ptr<NavigationDataBase>&, const std::string&) override;
};


class RoutesDataBase
{
public:
    RoutesDataBase()
    {
		routes_.reserve(2000);
    }

	void AddRoute(const std::string& bus_name, const std::shared_ptr<IRouteInfo>&);

	using RouteResponse = std::pair<std::string, 
    std::optional<RouteStats>>;

	void BuildAllRoutes(
		const std::shared_ptr<BusStopsDataBase>& stops_database, const std::shared_ptr<NavigationDataBase>&);

	[[nodiscard]] RouteResponse GetRouteStats(const std::string& bus) const;

private:
	std::unordered_map<std::string, 
    std::shared_ptr<IRouteInfo>> routes_;
};