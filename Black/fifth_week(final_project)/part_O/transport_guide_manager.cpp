#include "transport_guide_manager.h"
#include <string>
#include <iostream>
#include <fstream>
#include <string_view>
#include "json.h"


std::string ReadFileData(const std::string &file_name) {
    std::ifstream file(file_name, std::ios::binary | std::ios::ate);
    const std::ifstream::pos_type end_pos = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string data(end_pos, '\0');
    file.read(&data[0], end_pos);
    return data;
}

void TransportGuideManager::PerformReadQueries(std::istream &input) {
    const auto json_input = Json::Load(input).GetRoot();
    perform_read_queries(json_input.AsMap().at("base_requests").AsArray());
    routes_database_.BuildAllRoutes(stops_database_);
    save_base(json_input.AsMap().at("serialization_settings").AsMap().at("file").AsString());
}

void TransportGuideManager::save_base(const std::string &file_name) {
    Serializer::TransportCatalog transportCatalog;

    for (const auto&[stop_name, stop_stat] : stops_database_.GetStopsMap()) {
        Serializer::BusStopResponse stopResponse;
        stopResponse.mutable_buses_on_stop()->Reserve(stop_stat->GetStat().size());
        for (const auto &stat_param : stop_stat->GetStat()) {
            *stopResponse.mutable_buses_on_stop()->Add() = stat_param;
        }
        (*transportCatalog.mutable_stop_responses())[stop_name] = stopResponse;
    }

    for (const auto&[bus_name, bus_info] : routes_database_.GetRoutesMap()) {
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

void TransportGuideManager::PerformWriteQueries(std::istream &input, std::ostream &output) {
    deserializedCatalog = std::make_unique<Serializer::TransportCatalog>();
    const auto json_input = Json::Load(input).GetRoot();
    const std::string file_name =
            json_input.AsMap().at("serialization_settings").AsMap().at("file").AsString();
    const std::string data = ReadFileData(file_name);
    deserializedCatalog->ParseFromString(data);
    perform_write_queries(
            json_input.AsMap().at("stat_requests").AsArray(),
            output
    );
}

void TransportGuideManager::perform_read_queries(
        const std::vector<Json::Node> &post_requests) {
    for (const auto &request : post_requests) {
        if (request.AsMap().at("type").AsString() == "Stop") {
            BusStop stop;

            stop.name = request.AsMap().at("name").AsString();

            stop.coordinate = GroundPoint(request.AsMap().at("latitude").AsDouble(),
                                          request.AsMap().at("longitude").AsDouble());

            for (const auto &pair : request.AsMap().at("road_distances").AsMap()) {
                stop.distance_to_other_stops[pair.first] = pair.second.AsInt();
            }

            AddStop(stop);
        }

        if (request.AsMap().at("type").AsString() == "Bus") {
            std::shared_ptr<IRouteInfo> route_info;
            std::string bus_name = request.AsMap().at("name").AsString();

            if (request.AsMap().at("is_roundtrip").AsBool()) {
                route_info = std::make_shared<RoundRoute>();
            } else {
                route_info = std::make_shared<DirectRoute>();
            }

            for (const auto &stop : request.AsMap().at("stops").AsArray()) {
                route_info->AddStop(stop.AsString());
            }

            AddRoute(bus_name, route_info);
        }
    }
}

void TransportGuideManager::perform_write_queries(
        const std::vector<Json::Node> &get_requests, std::ostream &output) const {
    using namespace Json;

    std::vector<std::shared_ptr<JsonWriter>> result;

    for (const auto &request : get_requests) {
        std::map<std::string, std::shared_ptr<JsonWriter>> response;
        response["request_id"] =
                std::make_shared<JsonInt>(request.AsMap().at("id").AsInt());

        if (request.AsMap().at("type").AsString() == "Bus") {
            const auto route_response = deserializedCatalog->mutable_route_responses()->find(
                    request.AsMap().at("name").AsString()
            );

            if (route_response != deserializedCatalog->mutable_route_responses()->end()) {
                const auto stats = route_response->second;
                response["route_length"] =
                        std::make_shared<JsonInt>(stats.route_length());
                response["curvature"] =
                        std::make_shared<JsonDouble>(stats.curvature());
                response["stop_count"] =
                        std::make_shared<JsonInt>(stats.stops_on_route());
                response["unique_stop_count"] =
                        std::make_shared<JsonInt>(stats.unique_stops());
            } else {
                response["error_message"] =
                        std::make_shared<JsonString>(std::string("not found"));
            }
        }

        if (request.AsMap().at("type").AsString() == "Stop") {
            const auto stop_response =
                    deserializedCatalog->mutable_stop_responses()->find(request.AsMap().at("name").AsString());

            if (stop_response != deserializedCatalog->mutable_stop_responses()->end()) {
                std::vector<std::shared_ptr<JsonWriter>> buses;
                const auto buses_in_resp = stop_response->second.buses_on_stop();
                buses.reserve(buses_in_resp.size());

                for (const auto &bus : buses_in_resp) {
                    buses.push_back(
                            std::make_shared<JsonString>(bus));
                }

                response["buses"] = std::make_shared<JsonArray>(std::move(buses));
            } else {
                response["error_message"] =
                        std::make_shared<JsonString>(std::string("not found"));
            }
        }

        result.push_back(std::make_shared<JsonMap>(response));
    }

    output << JsonArray(result).ToString();
}

void TransportGuideManager::AddStop(const BusStop &stop) {
    stops_database_.AddStop(stop);
}

void TransportGuideManager::AddRoute(
        const std::string &bus_name, const std::shared_ptr<IRouteInfo> &route_info) {
    routes_database_.AddRoute(bus_name, route_info);
}