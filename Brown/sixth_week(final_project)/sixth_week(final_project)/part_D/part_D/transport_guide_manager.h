#pragma once
#include "routes_database.h"
#include "stops_database.h"
#include <memory>
#include "json.h"
#include <iostream>


class TransportGuideManager
{
public:
    TransportGuideManager()
    {
		stops_database_ = BusStopsDataBase();
		routes_database_ = RoutesDataBase();
    }

	void PerformQueries(std::istream& input = std::cin, 
		std::ostream& output = std::cout);

	void AddStop(const BusStop& stop);

	void AddRoute(const std::string&, 
		const std::shared_ptr<IRouteInfo>&);

	enum class ResponseType
	{
		BUS,
		STOP,
	};

private:
	BusStopsDataBase stops_database_;
	RoutesDataBase routes_database_;

	void perform_read_queries(
		const std::vector<Json::Node>& post_requests);

    void perform_write_queries(
		const std::vector<Json::Node>& get_requests, std::ostream& output) const;

	std::vector<std::string> logger;
};
