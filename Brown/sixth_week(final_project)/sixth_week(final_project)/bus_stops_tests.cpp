#include "transport_guide_tests.h"
#include "UnitTestsFramework.h"
#include "stops_database.h"
#include <sstream>


void TestReadGroundPoint()
{
	std::stringstream input("55.611087, 37.20829 55.595884, 37.209755 "
		"55.632761, 37.333324 55.574371, 37.6517 55.581065, 37.64839 55.587655, 37.645687 "
		"55.592028, 37.653656 55.580999, 37.659164");

	GroundPoint point;

	input >> point;
	ASSERT_EQUAL(point.latitude_, 55.611087);
	ASSERT_EQUAL(point.longitude_, 37.20829);

	input >> point;
	ASSERT_EQUAL(point.latitude_, 55.595884);
	ASSERT_EQUAL(point.longitude_, 37.209755);

	input >> point;
	ASSERT_EQUAL(point.latitude_, 55.632761);
	ASSERT_EQUAL(point.longitude_, 37.333324);

	input >> point;
	ASSERT_EQUAL(point.latitude_, 55.574371);
	ASSERT_EQUAL(point.longitude_, 37.6517);

	input >> point;
	ASSERT_EQUAL(point.latitude_, 55.581065);
	ASSERT_EQUAL(point.longitude_, 37.64839);

	input >> point;
	ASSERT_EQUAL(point.latitude_, 55.587655);
	ASSERT_EQUAL(point.longitude_, 37.645687);

	input >> point;
	ASSERT_EQUAL(point.latitude_, 55.592028);
	ASSERT_EQUAL(point.longitude_, 37.653656);

	input >> point;
	ASSERT_EQUAL(point.latitude_, 55.580999);
	ASSERT_EQUAL(point.longitude_, 37.659164);
}


void TestReadBusStop()
{
	BusStop stop;

	std::stringstream input("Tolstopaltsevo: 55.611087, 37.20829 "
		"Marushkino: 55.595884, 37.209755 "
		"Rasskazovka: 55.632761, 37.333324 "
		"Biryulyovo Zapadnoye: 55.574371, 37.6517 "
		"Biryusinka: 55.581065, 37.64839 "
		"Universam: 55.587655, 37.645687 "
		"Biryulyovo Tovarnaya: 55.592028, 37.653656 "
		"Biryulyovo Passazhirskaya: 55.580999, 37.659164");

	input >> stop;
	ASSERT_EQUAL(stop.name, "Tolstopaltsevo");
	ASSERT_EQUAL(stop.coordinate, GroundPoint(55.611087, 37.20829));

	input >> stop;
	ASSERT_EQUAL(stop.name, "Marushkino");
	ASSERT_EQUAL(stop.coordinate, GroundPoint(55.595884, 37.209755));

	input >> stop;
	ASSERT_EQUAL(stop.name, "Rasskazovka");
	ASSERT_EQUAL(stop.coordinate, GroundPoint(55.632761, 37.333324));

	input >> stop;
	ASSERT_EQUAL(stop.name, "Biryulyovo Zapadnoye");
	ASSERT_EQUAL(stop.coordinate, GroundPoint(55.574371, 37.6517));

	input >> stop;
	ASSERT_EQUAL(stop.name, "Biryusinka");
	ASSERT_EQUAL(stop.coordinate, GroundPoint(55.581065, 37.64839));

	input >> stop;
	ASSERT_EQUAL(stop.name, "Universam");
	ASSERT_EQUAL(stop.coordinate, GroundPoint(55.587655, 37.645687));

	input >> stop;
	ASSERT_EQUAL(stop.name, "Biryulyovo Tovarnaya");
	ASSERT_EQUAL(stop.coordinate, GroundPoint(55.592028, 37.653656));

	input >> stop;
	ASSERT_EQUAL(stop.name, "Biryulyovo Passazhirskaya");
	ASSERT_EQUAL(stop.coordinate, GroundPoint(55.580999, 37.659164));
}


void TestBusStopsDataBase()
{
	BusStopsDataBase stops_base;

	stops_base.AddStop(BusStop{ "Tolstopaltsevo", 
		GroundPoint(55.611087, 37.20829) });
	ASSERT_EQUAL(stops_base.bus_stops_.at("Tolstopaltsevo"), 
		GroundPoint(55.611087, 37.20829));

	stops_base.AddStop(BusStop{ "Marushkino",
		GroundPoint(55.595884, 37.209755) });
	ASSERT_EQUAL(stops_base.bus_stops_.at("Marushkino"),
		GroundPoint(55.595884, 37.209755));

	stops_base.AddStop(BusStop{ "Rasskazovka",
		GroundPoint(55.632761, 37.333324) });
	ASSERT_EQUAL(stops_base.bus_stops_.at("Rasskazovka"),
		GroundPoint(55.632761, 37.333324));

	stops_base.AddStop(BusStop{ "Biryulyovo Zapadnoye",
		GroundPoint(55.574371, 37.6517) });
	ASSERT_EQUAL(stops_base.bus_stops_.at("Biryulyovo Zapadnoye"),
		GroundPoint(55.574371, 37.6517));

	stops_base.AddStop(BusStop{ "Biryusinka",
		GroundPoint(55.581065, 37.64839) });
	ASSERT_EQUAL(stops_base.bus_stops_.at("Biryusinka"),
		GroundPoint(55.581065, 37.64839));

	stops_base.AddStop(BusStop{ "Universam",
		GroundPoint(55.587655, 37.645687) });
	ASSERT_EQUAL(stops_base.bus_stops_.at("Universam"),
		GroundPoint(55.587655, 37.645687));

	stops_base.AddStop(BusStop{ "Biryulyovo Tovarnaya",
		GroundPoint(55.592028, 37.653656) });
	ASSERT_EQUAL(stops_base.bus_stops_.at("Biryulyovo Tovarnaya"),
		GroundPoint(55.592028, 37.653656));

	stops_base.AddStop(BusStop{ "Biryulyovo Passazhirskaya",
		GroundPoint(55.580999, 37.659164) });
	ASSERT_EQUAL(stops_base.bus_stops_.at("Biryulyovo Passazhirskaya"),
		GroundPoint(55.580999, 37.659164));

	ASSERT_EQUAL(stops_base.bus_stops_.size(), size_t(8));
}


void TestBusStops()
{
	TestRunner tr;

	RUN_TEST(tr, TestReadGroundPoint);
	RUN_TEST(tr, TestReadBusStop);
	RUN_TEST(tr, TestBusStopsDataBase);
}