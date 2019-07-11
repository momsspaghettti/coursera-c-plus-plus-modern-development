#pragma once
#include "graph.h"
#include <string>
#include <unordered_map>
#include "router.h"
#include <memory>
#include <utility>
#include <string>
#include <variant>
#include <forward_list>


struct NavigationSettings
{
public:
	NavigationSettings()
	{
		bus_wait_time = 0;
		bus_velocity = 0;
	}

	NavigationSettings(unsigned time, double speed)
	{
		bus_wait_time = time;
		bus_velocity = speed * KPH_TO_MPM;
	}

	unsigned bus_wait_time;
	double bus_velocity;

private:
	const double KPH_TO_MPM = 1000.0 / 60.0;
};

class IEdge
{
public:
    enum class EdgeType
    {
        STOP,
        ROAD,
    };

	[[nodiscard]] virtual EdgeType GetEdgeType() const = 0;

	[[nodiscard]] virtual std::string GetEdgeInfoAsString() const = 0;

	[[nodiscard]] virtual size_t GetEdgeInfoAsId() const = 0;

	virtual ~IEdge() = default;
};


class StopEdge : public IEdge
{
public:
	StopEdge(size_t id) : stop_id_(id) {};

	[[nodiscard]] EdgeType GetEdgeType() const override
    {
		return EdgeType::STOP;
    }

	[[nodiscard]] std::string GetEdgeInfoAsString() const override
	{
		return "";
	}

	[[nodiscard]] size_t GetEdgeInfoAsId() const override
	{
		return stop_id_;
	}

private:
	const size_t stop_id_;
};


class RoadEdge : public IEdge
{
public:
	RoadEdge(std::string name) : bus_name_(std::move(name)) {};

	[[nodiscard]] EdgeType GetEdgeType() const override
    {
		return EdgeType::ROAD;
    }

	[[nodiscard]] std::string GetEdgeInfoAsString() const override
	{
		return bus_name_;
	}

	[[nodiscard]] size_t GetEdgeInfoAsId() const override
	{
		return 0;
	}

private:
	const std::string bus_name_;
};


class IRoadPart
{
public:
	[[nodiscard]] virtual std::pair<std::string, std::string> GetName() const = 0;

	[[nodiscard]] virtual std::string GetType() const = 0;

	[[nodiscard]] virtual double GetTime() const = 0;

	[[nodiscard]] virtual std::optional<unsigned> GetSpanCount() const = 0;

	virtual ~IRoadPart() = default;
};

class RoadStop : public IRoadPart
{
public:
	RoadStop(std::string stop_name, double time) : stop_name_(std::move(stop_name)), time_(time) {}

	[[nodiscard]] std::pair<std::string, std::string> GetName() const override
	{
		return std::make_pair("stop_name", stop_name_);
	}

	[[nodiscard]] std::string GetType() const override
	{
		return "Wait";
	}

	[[nodiscard]] double GetTime() const override
	{
		return time_;
	}

	[[nodiscard]] std::optional<unsigned> GetSpanCount() const override
	{
		return std::nullopt;
	}

private:
	const std::string stop_name_;
	const double time_;
};

class RoadBus : public IRoadPart
{
public:
	RoadBus(std::string bus_name, double time, unsigned span_count) :
		bus_name_(std::move(bus_name)), time_(time), span_count_(span_count) {}

	[[nodiscard]] std::pair<std::string, std::string> GetName() const override
	{
		return std::make_pair("bus", bus_name_);
	}

	[[nodiscard]] std::string GetType() const override
	{
		return "Bus";
	}

	[[nodiscard]] double GetTime() const override
	{
		return time_;
	}

	[[nodiscard]] std::optional<unsigned> GetSpanCount() const override
	{
		return span_count_;
	}

private:
	const std::string bus_name_;
	const double time_;
	const unsigned span_count_;
};

struct NavigationResponse
{
	NavigationResponse()
	{
		total_time = 0;
		items = std::vector<std::shared_ptr<IRoadPart>>();
	}

	double total_time;
	std::vector<std::shared_ptr<IRoadPart>> items;
};

class NavigationDataBase
{
public:
	NavigationDataBase(const NavigationSettings& settings, size_t stops_count) :
		settings_(settings), roads_graph_(std::make_shared<Graph::DirectedWeightedGraph<double>>(2 * stops_count))
	{
		stop_name_to_vertex_ids_.reserve(stops_count);
		vertex_id_to_stop_name_.reserve(2 * stops_count);
		edge_id_to_edge_info_.reserve(20 * stops_count);
	}

	void AddRoad(const std::string& from, const std::string& to,
		const std::string& bus_name, unsigned distances);

	void Build();

	[[nodiscard]] std::optional<NavigationResponse> GetDirections(
		const std::string& from, const std::string& to) const;

private:
	const NavigationSettings settings_;

    struct StopVertexes
    {
		StopVertexes() = default;

        StopVertexes(Graph::VertexId f, Graph::VertexId s)
        {
			me_first = f;
			me_second = s;
        }

		Graph::VertexId me_first;
		Graph::VertexId me_second;
    };

	std::shared_ptr<Graph::DirectedWeightedGraph<double>> roads_graph_;

	std::unordered_map<std::string, StopVertexes> stop_name_to_vertex_ids_;

	std::forward_list<std::string> stop_names_storage_;
    std::vector<std::forward_list<std::string>::const_iterator> vertex_id_to_stop_name_;

	std::vector<std::shared_ptr<IEdge>> edge_id_to_edge_info_;

	std::shared_ptr<Graph::Router<double>> router_;
};