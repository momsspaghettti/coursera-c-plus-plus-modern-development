#pragma once
#include "graph.h"
#include <string>
#include <unordered_map>
#include "router.h"
#include <memory>
#include <utility>
#include <string>
#include <variant>


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


struct BusStopInfo
{
	size_t index;
	unsigned length_to_next;
};


class BusInfo
{
public:
	void AddDirectStop(size_t index, unsigned length);

	void AddReverseStop(size_t index, unsigned length);

	[[nodiscard]] bool IsDirectBus() const;
	

	[[nodiscard]] const std::vector<BusStopInfo>& GetDirectStops() const;
	

	[[nodiscard]] const std::vector<BusStopInfo>& GetReverseStops() const;

private:
	std::vector<BusStopInfo> direct_stops_;
	std::vector<BusStopInfo> reverse_stops_;
};

struct EdgeInfo
{
	std::string bus;
	unsigned span_count;
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
		settings_(settings), roads_graph_(std::make_shared<Graph::DirectedWeightedGraph<double>>(stops_count))
	{
		stop_name_to_vertex_id_ = std::make_shared<std::unordered_map<std::string, Graph::VertexId>>();
		stop_name_to_vertex_id_->reserve(stops_count);

		vertex_id_to_stop_name_ = std::make_shared<std::vector<std::string>>();
	    vertex_id_to_stop_name_->reserve(2 * stops_count);

		edge_id_to_edge_info_ = std::make_shared<std::vector<EdgeInfo>>();
	    edge_id_to_edge_info_->reserve(400 * stops_count);

		bus_name_to_bus_info_.reserve(100);
	}

	void AddDirectStop(const std::string& bus_name, 
		const std::string& stop_name, unsigned distances_to_next);

    void AddReverseStop(const std::string& bus_name,
		const std::string& stop_name, unsigned distances_to_next);

	void Build();

	[[nodiscard]] std::optional<NavigationResponse> GetDirections(
		const std::string& from, const std::string& to) const;

private:
	const NavigationSettings settings_;

	std::shared_ptr<Graph::DirectedWeightedGraph<double>> roads_graph_;

	std::shared_ptr<std::unordered_map<std::string, Graph::VertexId>> stop_name_to_vertex_id_;

	std::shared_ptr<std::vector<std::string>> vertex_id_to_stop_name_;

	std::shared_ptr<std::vector<EdgeInfo>> edge_id_to_edge_info_;

	std::unordered_map<std::string, std::shared_ptr<BusInfo>> bus_name_to_bus_info_;

	std::shared_ptr<Graph::Router<double>> router_;

	void build_direct_route(const std::pair<std::string, std::shared_ptr<BusInfo>>&) const;
	void build_round_route(const std::pair<std::string, std::shared_ptr<BusInfo>>&) const;
};