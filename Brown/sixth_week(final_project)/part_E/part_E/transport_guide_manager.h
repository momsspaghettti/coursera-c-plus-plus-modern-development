#pragma once
#include "routes_database.h"
#include "stops_database.h"
#include <memory>
#include "json.h"
#include <iostream>
#include "navigation_database.h"


class TransportGuideManager
{
public:
    TransportGuideManager()
    {
		stops_database_ = std::make_shared<BusStopsDataBase>();
		routes_database_ = std::make_shared<RoutesDataBase>();
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

	void perform_read_queries(
		const std::vector<Json::Node>& post_requests) const;

    void perform_write_queries(
		const std::vector<Json::Node>& get_requests, std::ostream& output) const;
};
