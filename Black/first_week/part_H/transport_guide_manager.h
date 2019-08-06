#pragma once
#include "routes_database.h"
#include "stops_database.h"
#include <memory>
#include "json.h"
#include <iostream>
#include "navigation_database.h"
#include "map_database.h"


class TransportGuideManager
{
public:
    TransportGuideManager()
    {
		stops_database_ = std::make_shared<BusStopsDataBase>();
		routes_database_ = std::make_shared<RoutesDataBase>();
		map_database_ = std::make_shared<MapDataBase>();
    }

	void PerformQueries(std::istream& input = std::cin, 
		std::ostream& output = std::cout);

	void AddStop(const BusStop& stop) const;

	void AddRoute(const std::string&, 
		const std::shared_ptr<IRouteInfo>&) const;

private:
	std::shared_ptr<BusStopsDataBase> stops_database_;
	std::shared_ptr<RoutesDataBase> routes_database_;
	std::shared_ptr<NavigationDataBase> navigation_database_;
	std::shared_ptr<MapDataBase> map_database_;

	void build_transport_databases(const Json::Node& configs);

	void build_map_database(const Json::Node& configs);

	void perform_read_queries(
		const std::vector<Json::Node>& post_requests) const;

	void read_stop(const Json::Node& request) const;
	void read_bus(const Json::Node& request) const;

    void perform_write_queries(
		const std::vector<Json::Node>& get_requests, std::ostream& output) const;

    void perform_bus_query(const Json::Node&,
            std::map<std::string, std::shared_ptr<Json::JsonWriter>>&) const;
    void perform_stop_query(const Json::Node&,
            std::map<std::string, std::shared_ptr<Json::JsonWriter>>&) const;
    void perform_route_query(const Json::Node&,
            std::map<std::string, std::shared_ptr<Json::JsonWriter>>&) const;
    void perform_map_query(std::map<std::string, std::shared_ptr<Json::JsonWriter>>&) const;
};
