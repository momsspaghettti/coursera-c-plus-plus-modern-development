#include "navigation_database.h"


void NavigationDataBase::AddRoad(const std::string& from,
	const std::string& to, const std::string& bus_name, unsigned distances)
{
	Graph::VertexId first_from_id, second_from_id,
    first_to_id, second_to_id;

	const auto from_finder = stop_name_to_vertex_ids_.find(from);
	if (from_finder != stop_name_to_vertex_ids_.end())
	{
		first_from_id = from_finder->second.me_first;
		second_from_id = from_finder->second.me_second;
	} 
    else
	{
		stop_names_storage_.push_front(from);

		vertex_id_to_stop_name_.push_back(stop_names_storage_.begin());
		vertex_id_to_stop_name_.push_back(stop_names_storage_.begin());

		first_from_id = vertex_id_to_stop_name_.size() - 2;
		second_from_id = vertex_id_to_stop_name_.size() - 1;

		stop_name_to_vertex_ids_[from] = StopVertexes{ first_from_id, second_from_id };
	}

	const auto to_finder = stop_name_to_vertex_ids_.find(to);
	if (to_finder != stop_name_to_vertex_ids_.end())
	{
		first_to_id = to_finder->second.me_first;
		second_to_id = to_finder->second.me_second;
	} 
    else
	{
		stop_names_storage_.push_front(to);

		vertex_id_to_stop_name_.push_back(stop_names_storage_.begin());
		vertex_id_to_stop_name_.push_back(stop_names_storage_.begin());

		first_to_id = vertex_id_to_stop_name_.size() - 2;
		second_to_id = vertex_id_to_stop_name_.size() - 1;

		stop_name_to_vertex_ids_[to] = StopVertexes{ first_to_id, second_to_id };
	}

	const Graph::Edge<double> edge1 = { first_from_id, second_from_id, 
	    static_cast<double>(settings_.bus_wait_time) };
	const Graph::Edge<double> edge2 = { second_from_id, first_to_id,
		distances / settings_.bus_velocity };
	const Graph::Edge<double> edge3 = { first_from_id, first_to_id,
		distances / settings_.bus_velocity };

	roads_graph_->AddEdge(edge1);
	roads_graph_->AddEdge(edge2);
	roads_graph_->AddEdge(edge3);

	edge_id_to_edge_info_.push_back(std::make_shared<StopEdge>(first_from_id));
	edge_id_to_edge_info_.push_back(std::make_shared<RoadEdge>(bus_name));
	edge_id_to_edge_info_.push_back(std::make_shared<RoadEdge>(bus_name));
}


void NavigationDataBase::Build()
{
	router_ = std::make_shared<Graph::Router<double>>(*roads_graph_);
}


std::optional<NavigationResponse> NavigationDataBase::GetDirections(
	const std::string& from, const std::string& to) const
{
	if (from == to)
	{
		return NavigationResponse();
	}

	const auto from_id = stop_name_to_vertex_ids_.at(from);
	const auto to_id = stop_name_to_vertex_ids_.at(to);

	const auto route_info = router_->BuildRoute(from_id.me_second, to_id.me_first);

	if (route_info)
	{
		NavigationResponse result;
		result.total_time = route_info->weight + settings_.bus_wait_time;
		result.items.reserve(2 * route_info->edge_count);

		result.items.push_back(std::make_shared<RoadStop>(
			from, settings_.bus_wait_time));

		double current_bus_time = 0;
		unsigned current_bus_span_count = 0;
		for (size_t i = 0; i < route_info->edge_count; ++i)
		{
			const auto edge_id = router_->GetRouteEdge(route_info->id, i);
			const auto edge = roads_graph_->GetEdge(edge_id);

            if (edge_id_to_edge_info_[edge_id]->GetEdgeType() == IEdge::EdgeType::STOP)
            {
                if (current_bus_time != 0)
                {
					result.items.push_back(std::make_shared<RoadBus>(
						edge_id_to_edge_info_[edge_id]->GetEdgeInfoAsString(), 
						current_bus_time, current_bus_span_count));
                }

				current_bus_time = 0;
				current_bus_span_count = 0;

				result.items.push_back(std::make_shared<RoadStop>(
					*vertex_id_to_stop_name_[edge_id_to_edge_info_[edge_id]->GetEdgeInfoAsId()], 
					settings_.bus_wait_time));
            }
			else
			{
				current_bus_time += edge.weight;
				++current_bus_span_count;
			}
		}

		if (current_bus_time != 0)
		{
			result.items.push_back(std::make_shared<RoadBus>(
				edge_id_to_edge_info_[router_->GetRouteEdge(route_info->id, 
					route_info->edge_count - 1)]->GetEdgeInfoAsString(),
				current_bus_time, current_bus_span_count));
		}		

		return result;
	} 

	return std::nullopt;
}