#pragma once
#include <string>
#include <iostream>
#include <string>
#include <unordered_map>


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

private:
	double latitude_, longitude_;

	constexpr static double PI = 3.1415926535;
	constexpr static double EARTH_RADIUS = 6371000;

	friend std::istream& operator>>(std::istream& input, 
		GroundPoint& point);

	friend std::ostream& operator<<(std::ostream& output,
		const GroundPoint& point);

	friend bool operator!=(const GroundPoint& lhs, 
		const GroundPoint& rhs);

	friend void TestReadGroundPoint();
};


struct BusStop
{
	std::string name;

	GroundPoint coordinate;
};


std::istream& operator>>(std::istream& input, BusStop& bus_stop);


class BusStopsDataBase
{
public:
	BusStopsDataBase() = default;

	void AddStop(const BusStop& bus_stop);

	[[nodiscard]] double ComputeDistanceBetweenStops(
		const std::string& from, const std::string& to) const;

private:
	std::unordered_map<std::string, GroundPoint> bus_stops_;

	friend void TestBusStopsDataBase();
};