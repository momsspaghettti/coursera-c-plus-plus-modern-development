#include <iostream>
#include <fstream>
#include "database_keeper.h"
#include "transport_guide_manager.h"
#include "transport_catalog.pb.h"


std::string ReadFileData(const std::string &file_name) {
    std::ifstream file(file_name, std::ios::binary | std::ios::ate);
    const std::ifstream::pos_type end_pos = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string data(end_pos, '\0');
    file.read(&data[0], end_pos);
    return data;
}

void DataBaseKeeper::SaveDataBase(
        const std::string &file_name,
        const TransportGuideManager *base
) {
    Serializer::TransportCatalog transportCatalog;

    for (const auto&[stop_name, stop_stat] : base->stops_database_->GetStopsMap()) {
        Serializer::BusStopResponse stopResponse;
        stopResponse.mutable_buses_on_stop()->Reserve(stop_stat->GetStat().size());
        for (const auto &stat_param : stop_stat->GetStat()) {
            *stopResponse.mutable_buses_on_stop()->Add() = stat_param;
        }
        (*transportCatalog.mutable_stop_responses())[stop_name] = stopResponse;
    }

    for (const auto&[bus_name, bus_info] : base->routes_database_->GetRoutesMap()) {
        const auto&[
        stops_on_route,
        unique_stops,
        route_length,
        direct_distance,
        curvature
        ] = bus_info->GetRouteStats();

        Serializer::RouteResponse routeResponse;
        routeResponse.set_stops_on_route(stops_on_route);
        routeResponse.set_unique_stops(unique_stops);
        routeResponse.set_route_length(route_length);
        routeResponse.set_curvature(curvature);
        (*transportCatalog.mutable_route_responses())[bus_name] = routeResponse;
    }

    std::ofstream file(file_name, std::ios::binary | std::ios::ate);
    transportCatalog.SerializeToOstream(&file);
    file.close();
}

void DataBaseKeeper::LoadDataBase(const std::string &file_name) {
    deserialized_catalog_ = std::make_unique<Serializer::TransportCatalog>();
    const std::string data = ReadFileData(file_name);
    deserialized_catalog_->ParseFromString(data);
}