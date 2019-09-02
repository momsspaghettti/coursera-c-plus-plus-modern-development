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
    serialized_catalog_ = std::make_unique<Serializer::TransportCatalog>();
    save_buses(base);
    save_stops(base);
    save_navigation_data_base(base);

    std::ofstream file(file_name, std::ios::binary | std::ios::ate);
    serialized_catalog_->SerializeToOstream(&file);
    file.close();
}

void DataBaseKeeper::save_buses(const TransportGuideManager *base) {
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
        (*serialized_catalog_->mutable_route_responses())[bus_name] = routeResponse;
    }
}

void DataBaseKeeper::save_stops(const TransportGuideManager *base) {
    for (const auto&[stop_name, stop_stat] : base->stops_database_->GetStopsMap()) {
        Serializer::BusStopResponse stopResponse;
        stopResponse.mutable_buses_on_stop()->Reserve(stop_stat->GetStat().size());
        for (const auto &stat_param : stop_stat->GetStat()) {
            *stopResponse.mutable_buses_on_stop()->Add() = stat_param;
        }
        (*serialized_catalog_->mutable_stop_responses())[stop_name] = stopResponse;
    }
}

void DataBaseKeeper::save_navigation_data_base(const TransportGuideManager *base) {
    Serializer::NavigationSettings settings;
    settings.set_bus_wait_time(
            base->navigation_database_->settings_.bus_wait_time
            );
    settings.set_bus_velocity(
            base->navigation_database_->settings_.bus_velocity
            );
    *serialized_catalog_->mutable_navigation_database()->mutable_settings() = settings;

    for (const auto& [stop_name, vertex_id] : *base->navigation_database_->stop_name_to_vertex_id_) {
        (*serialized_catalog_->mutable_navigation_database()->mutable_stop_name_to_vertex_id())[stop_name] =
                vertex_id;
    }

    serialized_catalog_->mutable_navigation_database()->mutable_vertex_id_to_stop_name()->Reserve(
            base->navigation_database_->vertex_id_to_stop_name_->size()
            );
    for (const auto& stop_name : *base->navigation_database_->vertex_id_to_stop_name_) {
        *serialized_catalog_->mutable_navigation_database()->mutable_vertex_id_to_stop_name()->Add() =
                stop_name;
    }

    serialized_catalog_->mutable_navigation_database()->mutable_edge_id_to_edge_info()->Reserve(
            base->navigation_database_->edge_id_to_edge_info_->size()
            );
    for (const auto& edge_info : *base->navigation_database_->edge_id_to_edge_info_) {
        Serializer::EdgeInfo edgeInfo;
        *edgeInfo.mutable_bus() = edge_info.bus;
        edgeInfo.set_span_count(edge_info.span_count);
        *serialized_catalog_->mutable_navigation_database()->mutable_edge_id_to_edge_info()->Add() =
                edgeInfo;
    }

    serialized_catalog_->mutable_navigation_database()->mutable_roads_graph()->mutable_edges()->Reserve(
            base->navigation_database_->roads_graph_->GetEdges().size()
            );
    for (const auto& [from, to, weight] : base->navigation_database_->roads_graph_->GetEdges()) {
        Serializer::Edge edge;
        edge.set_from(from);
        edge.set_to(to);
        edge.set_weight(weight);
        *serialized_catalog_->mutable_navigation_database()->mutable_roads_graph()->mutable_edges()->Add() =
                edge;
    }

    serialized_catalog_->mutable_navigation_database()->mutable_roads_graph()->mutable_incidence_lists()->Reserve(
            base->navigation_database_->roads_graph_->GetIncidenceLists().size()
            );
    for (const auto& incidence_list : base->navigation_database_->roads_graph_->GetIncidenceLists()) {
        Serializer::IncidenceList incidenceList;
        incidenceList.mutable_incidence_list()->Reserve(incidence_list.size());
        for (const auto& incidence : incidence_list) {
            *incidenceList.mutable_incidence_list()->Add() = incidence;
        }
        *serialized_catalog_->mutable_navigation_database()->mutable_roads_graph()->mutable_incidence_lists()->Add() =
                incidenceList;
    }
}

void DataBaseKeeper::LoadDataBase(const std::string &file_name) {
    deserialized_catalog_ = std::make_unique<Serializer::TransportCatalog>();
    const std::string data = ReadFileData(file_name);
    deserialized_catalog_->ParseFromString(data);
}