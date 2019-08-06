#include "transport_guide_manager.h"
#include <sstream>
#include <string>
#include <string_view>
#include "json.h"


void TransportGuideManager::PerformQueries(std::istream& input, 
	std::ostream& output)
{
	input.tie(nullptr);
	std::ios_base::sync_with_stdio(false);
	output.precision(6);

	const auto json_input = Json::Load(input).GetRoot();

	perform_read_queries(json_input.AsMap().at("base_requests").AsArray());

	build_transport_databases(json_input);

	if (
	        const auto finder = json_input.AsMap().find("render_settings");
	        finder != json_input.AsMap().end()
	        )
	    build_map_database(json_input);

	perform_write_queries(json_input.AsMap().at("stat_requests").AsArray(), output);
}


void TransportGuideManager::perform_read_queries(
	const std::vector<Json::Node>& post_requests) const
{	
    for (const auto& request : post_requests)
    {
        if (request.AsMap().at("type").AsString() == "Stop")
            read_stop(request);

        if (request.AsMap().at("type").AsString() == "Bus")
            read_bus(request);
    }
}


void TransportGuideManager::read_stop(const Json::Node &request) const {
    BusStop stop;

    stop.name = request.AsMap().at("name").AsString();

    stop.coordinate = GroundPoint(request.AsMap().at("latitude").AsDouble(),
                                  request.AsMap().at("longitude").AsDouble());

    for (const auto& pair : request.AsMap().at("road_distances").AsMap())
    {
        stop.distance_to_other_stops[pair.first] = pair.second.AsInt();
    }

    AddStop(stop);
}


void TransportGuideManager::read_bus(const Json::Node &request) const {
    std::shared_ptr<IRouteInfo> route_info;
    std::string bus_name = request.AsMap().at("name").AsString();

    if (request.AsMap().at("is_roundtrip").AsBool())
    {
        route_info = std::make_shared<RoundRoute>();
    }
    else
    {
        route_info = std::make_shared<DirectRoute>();
    }

    for (const auto& stop : request.AsMap().at("stops").AsArray())
    {
        route_info->AddStop(stop.AsString());
    }

    AddRoute(bus_name, route_info);
}


void TransportGuideManager::build_transport_databases(const Json::Node &configs) {
    NavigationSettings settings(
            configs.AsMap().at("routing_settings").AsMap().at("bus_wait_time").AsInt(),
            configs.AsMap().at("routing_settings").AsMap().at("bus_velocity").AsDouble());

    navigation_database_ =
            std::make_shared<NavigationDataBase>(settings, stops_database_->GetBusStopsCount());

    routes_database_->BuildAllRoutes(stops_database_, navigation_database_);
    navigation_database_->Build();
}


void TransportGuideManager::build_map_database(const Json::Node& configs) {
    map_database_->AddRenderSettings(configs.AsMap().at("render_settings").AsMap());
    map_database_->AddMinMaxCoords(stops_database_->GetMinMaxCoords());
    map_database_->BuildMap(stops_database_, routes_database_);
}


void TransportGuideManager::perform_write_queries(
	const std::vector<Json::Node>& get_requests, std::ostream& output) const
{
	using namespace Json;

	std::vector<std::shared_ptr<JsonWriter>> result;

    for (const auto& request : get_requests)
    {
		std::map<std::string, std::shared_ptr<JsonWriter>> response;
		response["request_id"] =
			std::make_shared<JsonInt>(request.AsMap().at("id").AsInt());

		if (request.AsMap().at("type").AsString() == "Bus")
		    perform_bus_query(request, response);

		if (request.AsMap().at("type").AsString() == "Stop")
		    perform_stop_query(request, response);

		if (request.AsMap().at("type").AsString() == "Route")
		    perform_route_query(request, response);

		if (request.AsMap().at("type").AsString() == "Map")
		    perform_map_query(response);

		result.push_back(std::make_shared<JsonMap>(response));
    }

	output << JsonArray(result).ToString();
}


void TransportGuideManager::perform_bus_query(
        const Json::Node &request,
        std::map<std::string, std::shared_ptr<Json::JsonWriter>> &response
        ) const {

    const auto route_response = routes_database_->GetRouteStats(
            request.AsMap().at("name").AsString());

    if (route_response.second)
    {
        response["route_length"] =
                std::make_shared<Json::JsonInt>(route_response.second->route_length);
        response["curvature"] =
                std::make_shared<Json::JsonDouble>(route_response.second->curvature);
        response["stop_count"] =
                std::make_shared<Json::JsonInt>(route_response.second->stops_on_route);
        response["unique_stop_count"] =
                std::make_shared<Json::JsonInt>(route_response.second->unique_stops);
    } else {
        response["error_message"] =
                std::make_shared<Json::JsonString>(std::string("not found"));
    }
}


void TransportGuideManager::perform_stop_query(
        const Json::Node &request,
        std::map<std::string, std::shared_ptr<Json::JsonWriter>> &response
        ) const {

    const auto stop_response = stops_database_->GetBusStopStat(
            request.AsMap().at("name").AsString());

    if (stop_response.second)
    {
        std::vector<std::shared_ptr<Json::JsonWriter>> buses;
        buses.reserve(stop_response.second->size());

        for (const auto& bus : *stop_response.second)
        {
            buses.push_back(
                    std::make_shared<Json::JsonString>(bus));
        }

        response["buses"] = std::make_shared<Json::JsonArray>(buses);
    } else {
        response["error_message"] =
                std::make_shared<Json::JsonString>(std::string("not found"));
    }
}


void TransportGuideManager::perform_route_query(
        const Json::Node &request,
        std::map<std::string, std::shared_ptr<Json::JsonWriter>> &response
        ) const {

    const auto navigation_response =
            navigation_database_->GetDirections(request.AsMap().at("from").AsString(),
                                                request.AsMap().at("to").AsString());

    if (navigation_response)
    {
        response["total_time"] =
                std::make_shared<Json::JsonDouble>(navigation_response->total_time);

        std::vector<std::shared_ptr<Json::JsonWriter>> items;

        for (const auto& item : navigation_response->items)
        {
            std::map<std::string, std::shared_ptr<Json::JsonWriter>> item_map;

            item_map["type"] = std::make_shared<Json::JsonString>(item->GetType());
            item_map[item->GetName().first] = std::make_shared<Json::JsonString>(item->GetName().second);
            item_map["time"] = std::make_shared<Json::JsonDouble>(item->GetTime());

            if (item->GetType() == "Bus")
            {
                item_map["span_count"] = std::make_shared<Json::JsonInt>(*item->GetSpanCount());
            }

            items.push_back(std::make_shared<Json::JsonMap>(item_map));
        }

        response["items"] = std::make_shared<Json::JsonArray>(items);
    } else {
        response["error_message"] =
                std::make_shared<Json::JsonString>(std::string("not found"));
    }
}


void TransportGuideManager::perform_map_query(
        std::map<std::string, std::shared_ptr<Json::JsonWriter>> &response
        ) const {
    response["map"] =
            std::make_shared<Json::JsonString>(map_database_->GetMapResponse());
}


void TransportGuideManager::AddStop(const BusStop& stop) const
{
	stops_database_->AddStop(stop);
}


void TransportGuideManager::AddRoute(
	const std::string& bus_name, const std::shared_ptr<IRouteInfo>& route_info) const
{
	routes_database_->AddRoute(bus_name, route_info);
}