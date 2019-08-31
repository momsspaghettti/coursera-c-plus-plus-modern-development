#pragma once

#include "routes_database.h"
#include "stops_database.h"
#include "transport_catalog.pb.h"
#include <memory>
#include "json.h"
#include <iostream>


class TransportGuideManager {
public:
    TransportGuideManager() {
        stops_database_ = BusStopsDataBase();
        routes_database_ = RoutesDataBase();
    }

    void PerformReadQueries(std::istream &input = std::cin);

    void PerformWriteQueries(std::istream &input = std::cin, std::ostream &output = std::cout);

    void AddStop(const BusStop &stop);

    void AddRoute(const std::string &,
                  const std::shared_ptr<IRouteInfo> &);

private:
    BusStopsDataBase stops_database_;
    RoutesDataBase routes_database_;

    std::unique_ptr<Serializer::TransportCatalog> deserializedCatalog;

    void perform_read_queries(
            const std::vector<Json::Node> &post_requests);

    void perform_write_queries(
            const std::vector<Json::Node> &get_requests, std::ostream &output) const;

    void save_base(const std::string &file_name);
};
