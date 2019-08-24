#pragma once
#include <string>
#include <string>
#include <set>
#include <optional>
#include <memory>
#include <unordered_map>
#include <unordered_set>


class GroundPoint
{
public:
    GroundPoint()
    {
		latitude_ = longitude_ = 0;
    }

    GroundPoint(double lat, double lon) : 
    latitude_(lat), longitude_(lon) {}

	static double ComputeDistance(const GroundPoint& lhs, 
		const GroundPoint& rhs);

    [[nodiscard]] double GetLatitude() const {
        return latitude_;
    }

    [[nodiscard]] double GetLongitude() const {
        return longitude_;
    }

private:
	double latitude_, longitude_;

	constexpr static double PI = 3.1415926535;
	constexpr static double EARTH_RADIUS = 6371000;

	friend bool operator!=(const GroundPoint& lhs, 
		const GroundPoint& rhs);
};


struct BusStop
{
	std::string name;
	GroundPoint coordinate;
	std::unordered_map<std::string, unsigned> distance_to_other_stops;
};


class BusStopStats
{
public:
	BusStopStats() = default;

    void AddBusInStat(const std::string& bus);

	[[nodiscard]] const std::set<std::string>& GetStat() const;

	[[nodiscard]] const std::unordered_map<std::string, unsigned>& 
		GetDistanceToOtherStops() const;

	void SetCoordinate(const GroundPoint& coordinate);

	void SetDistanceToOtherStops(const std::unordered_map<std::string, unsigned>& other_stops);

	[[nodiscard]] const GroundPoint& GetCoordinate() const;

private:
	GroundPoint coordinate_;

	std::set<std::string> buses_on_stop_;

	std::unordered_map<std::string, unsigned> distance_to_other_stops_;
};


class BusStopsDataBase
{
public:
	BusStopsDataBase() {
	    single_stops_ = std::make_unique<std::unordered_set<std::string>>();
	    bus_stops_.reserve(100);
	}

	void AddStop(const BusStop& bus_stop);

	[[nodiscard]] double ComputeDirectDistanceBetweenStops(
		const std::string& from, const std::string& to) const;

	[[nodiscard]] unsigned ComputeRealDistanceBetweenStops(
		const std::string& from, const std::string& to) const;

    void AddBusOnStop(const std::string& bus, const std::string& stop);

	using BusStopResponse = std::pair<std::string, 
    std::optional<std::set<std::string>>>;

	[[nodiscard]] BusStopResponse GetBusStopStat(const std::string& stop) const;

    [[nodiscard]] size_t GetBusStopsCount() const;

    [[nodiscard]] const std::unordered_map<std::string,
            std::shared_ptr<BusStopStats>>& GetStopsMap() const {
        return bus_stops_;
    }

    [[nodiscard]] const std::set<std::string>& GetBuses(const std::string& stop_name) const {
        return bus_stops_.at(stop_name)->GetStat();
    }

    [[nodiscard]] const std::shared_ptr<BusStopStats>& GetStopInfo(const std::string& stop_name) const {
        return bus_stops_.at(stop_name);
    }

    [[nodiscard]] const std::unordered_set<std::string>& GetSingleStops() const {
        return *single_stops_;
    }

private:
	std::unordered_map<std::string, 
    std::shared_ptr<BusStopStats>> bus_stops_;

    std::unique_ptr<std::unordered_set<std::string>> single_stops_;
};