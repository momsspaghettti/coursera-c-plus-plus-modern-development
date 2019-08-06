#include "navigation_database.h"
#include <cstdint>


void BusInfo::AddDirectStop(size_t index, unsigned length)
{
	direct_stops_.push_back(BusStopInfo{ index, length });
}


void BusInfo::AddReverseStop(size_t index, unsigned length)
{
	reverse_stops_.push_back(BusStopInfo{ index, length });
}


bool BusInfo::IsDirectBus() const
{
	return !reverse_stops_.empty();
}


const std::vector<BusStopInfo>& BusInfo::GetDirectStops() const
{
	return direct_stops_;
}


const std::vector<BusStopInfo>& BusInfo::GetReverseStops() const
{
	return reverse_stops_;
}


void NavigationDataBase::AddDirectStop(const std::string& bus_name, 
	const std::string& stop_name, unsigned distances_to_next)
{
	const auto finder = stop_name_to_vertex_id_->find(stop_name);

    if (finder != stop_name_to_vertex_id_->end())
    {
		auto& bus_info = bus_name_to_bus_info_[bus_name];

		if (!bus_info)
			bus_info = std::make_shared<BusInfo>();

		bus_info->AddDirectStop(finder->second, distances_to_next);
    }
    else
    {
		vertex_id_to_stop_name_->push_back(stop_name);
		stop_name_to_vertex_id_->insert({stop_name, vertex_id_to_stop_name_->size() - 1});

		auto& bus_info = bus_name_to_bus_info_[bus_name];

		if (!bus_info)
			bus_info = std::make_shared<BusInfo>();

		bus_info->AddDirectStop(vertex_id_to_stop_name_->size() - 1, distances_to_next);
    }
}


void NavigationDataBase::AddReverseStop(const std::string& bus_name, 
	const std::string& stop_name, unsigned distances_to_next)
{
	const auto finder = stop_name_to_vertex_id_->find(stop_name);

	bus_name_to_bus_info_[bus_name]->AddReverseStop(finder->second, distances_to_next);
}


void NavigationDataBase::Build()
{
    for (const auto& bus_pair : bus_name_to_bus_info_)
    {
        if (bus_pair.second->IsDirectBus())
        {
			build_direct_route(bus_pair);
        }
        else
        {
			build_round_route(bus_pair);
        }
    }

	router_ = std::make_shared<Graph::Router<double>>(*roads_graph_);
}


void NavigationDataBase::build_direct_route(
	const std::pair<std::string, std::shared_ptr<BusInfo>>& bus_pair) const
{
    for (size_t i = 0; i < bus_pair.second->GetDirectStops().size() - 1; ++i)
    {
		uint64_t adder = bus_pair.second->GetDirectStops()[i].length_to_next;
		unsigned counter = 0;
        
        for (size_t j = i + 1; j < bus_pair.second->GetDirectStops().size(); ++j)
        {
			++counter;

			const auto from_id = bus_pair.second->GetDirectStops()[i].index;
			const auto to_id = bus_pair.second->GetDirectStops()[j].index;

			roads_graph_->AddEdge(Graph::Edge<double>{from_id, to_id, 
				settings_.bus_wait_time + adder / settings_.bus_velocity});

			edge_id_to_edge_info_->push_back(EdgeInfo{bus_pair.first, counter});

            if (j + 1 != bus_pair.second->GetDirectStops().size())
                adder += bus_pair.second->GetDirectStops()[j].length_to_next;
        }

        for (size_t j = 0; j < bus_pair.second->GetReverseStops().size() - 1; ++j)
        {
			++counter;

			adder += bus_pair.second->GetReverseStops()[j].length_to_next;

			const auto from_id = bus_pair.second->GetDirectStops()[i].index;
			const auto to_id = bus_pair.second->GetReverseStops()[j + 1].index;

			roads_graph_->AddEdge(Graph::Edge<double>{from_id, to_id,
				settings_.bus_wait_time + adder / settings_.bus_velocity});

			edge_id_to_edge_info_->push_back(EdgeInfo{bus_pair.first, counter});
        }
    }

	for (size_t i = 0; i < bus_pair.second->GetReverseStops().size() - 1; ++i)
	{
		uint64_t adder = bus_pair.second->GetReverseStops()[i].length_to_next;
		unsigned counter = 0;

		for (size_t j = i + 1; j < bus_pair.second->GetReverseStops().size(); ++j)
		{
			++counter;

			const auto from_id = bus_pair.second->GetReverseStops()[i].index;
			const auto to_id = bus_pair.second->GetReverseStops()[j].index;

			roads_graph_->AddEdge(Graph::Edge<double>{from_id, to_id,
				settings_.bus_wait_time + adder / settings_.bus_velocity});

			edge_id_to_edge_info_->push_back(EdgeInfo{bus_pair.first, counter });

			if (j + 1 != bus_pair.second->GetReverseStops().size())
				adder += bus_pair.second->GetReverseStops()[j].length_to_next;
		}
	}
}



void NavigationDataBase::build_round_route(
	const std::pair<std::string, std::shared_ptr<BusInfo>>& bus_pair) const
{
    for (size_t i = 0; i < bus_pair.second->GetDirectStops().size() - 1; ++i)
    {
		uint64_t adder = bus_pair.second->GetDirectStops()[i].length_to_next;
		unsigned counter = 0;

		for (size_t j = i + 1; j < bus_pair.second->GetDirectStops().size(); ++j)
		{
			++counter;

			const auto from_id = bus_pair.second->GetDirectStops()[i].index;
			const auto to_id = bus_pair.second->GetDirectStops()[j].index;

			roads_graph_->AddEdge(Graph::Edge<double>{from_id, to_id,
				settings_.bus_wait_time + adder / settings_.bus_velocity});

			edge_id_to_edge_info_->push_back(EdgeInfo{bus_pair.first, counter });

			if (j + 1 != bus_pair.second->GetDirectStops().size())
				adder += bus_pair.second->GetDirectStops()[j].length_to_next;
		}
    }
}


std::optional<NavigationResponse> NavigationDataBase::GetDirections(
	const std::string& from, const std::string& to) const
{
	if (from == to)
	{
		return NavigationResponse();
	}

	const auto from_finder = stop_name_to_vertex_id_->find(from);
	const auto to_finder = stop_name_to_vertex_id_->find(to);

	if (from_finder == stop_name_to_vertex_id_->end() || to_finder == stop_name_to_vertex_id_->end())
		return std::nullopt;

	const auto from_id = from_finder->second;
	const auto to_id = to_finder->second;

	const auto route_info = router_->BuildRoute(from_id, to_id);

	if (route_info)
	{
		NavigationResponse result;
		result.total_time = route_info->weight;
		result.items.reserve(2 * route_info->edge_count);

		for (size_t i = 0; i < route_info->edge_count; ++i)
		{
			const auto edge_id = router_->GetRouteEdge(route_info->id, i);
			const auto edge = roads_graph_->GetEdge(edge_id);

			result.items.push_back(std::make_shared<RoadStop>(
				vertex_id_to_stop_name_->at(edge.from), settings_.bus_wait_time));

			result.items.push_back(std::make_shared<RoadBus>(
				edge_id_to_edge_info_->at(edge_id).bus, 
				edge.weight - settings_.bus_wait_time, 
				edge_id_to_edge_info_->at(edge_id).span_count));
		}	

		return result;
	} 

	return std::nullopt;
}