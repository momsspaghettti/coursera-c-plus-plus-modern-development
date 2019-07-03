#include "routes_database.h"
#include "UnitTestsFramework.h"
#include "routes_database_tests.h"


void TestReadRouteFromString()
{
	std::string input = "256: Biryulyovo Zapadnoye > "
		"Biryusinka > Universam > Biryulyovo Tovarnaya > "
		"Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye";

	RoutesDataBase data_base;

	data_base.ReadRouteFromString(input);
	
	std::vector<std::string> expected{ "Biryulyovo Zapadnoye", "Biryusinka", "Universam", 
	    "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye" };
	ASSERT_EQUAL(data_base.routes_["256"].stops_, expected);

	input = "750: Tolstopaltsevo - Marushkino - Rasskazovka";
	expected = std::vector < std::string>{ "Tolstopaltsevo", "Marushkino", "Rasskazovka" };

	data_base.ReadRouteFromString(input);
	ASSERT_EQUAL(data_base.routes_["750"].stops_, expected);
}


void TestRoutesDataBase()
{
	BusStop stop;
	BusStopsDataBase stops_base;

	std::stringstream input("Tolstopaltsevo: 55.611087, 37.20829 "
		"Marushkino: 55.595884, 37.209755 "
		"Rasskazovka: 55.632761, 37.333324 "
		"Biryulyovo Zapadnoye: 55.574371, 37.6517 "
		"Biryusinka: 55.581065, 37.64839 "
		"Universam: 55.587655, 37.645687 "
		"Biryulyovo Tovarnaya: 55.592028, 37.653656 "
		"Biryulyovo Passazhirskaya: 55.580999, 37.659164");

    while (input)
    {
		input >> stop;
		stops_base.AddStop(stop);
    }

	RoutesDataBase routes_data_base;

	std::string str_input = "256: Biryulyovo Zapadnoye > "
		"Biryusinka > Universam > Biryulyovo Tovarnaya > "
		"Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye";

	routes_data_base.ReadRouteFromString(str_input);

    str_input = "750: Tolstopaltsevo - Marushkino - Rasskazovka";

	routes_data_base.ReadRouteFromString(str_input);

	routes_data_base.BuildAllRoutes(stops_base);

	auto response = routes_data_base.GetRouteStats("256");
	ASSERT_EQUAL(response.first, "256");
	ASSERT_EQUAL(response.second->stops_on_route, 6);
	ASSERT_EQUAL(response.second->unique_stops, 5);
	ASSERT(std::abs(response.second->route_length - 4371.02) < 0.1);

	response = routes_data_base.GetRouteStats("750");
	ASSERT_EQUAL(response.first, "750");
	ASSERT_EQUAL(response.second->stops_on_route, 5);
	ASSERT_EQUAL(response.second->unique_stops, 3);
	ASSERT(std::abs(response.second->route_length - 20939.5) < 0.1);

	response = routes_data_base.GetRouteStats("751");
	ASSERT_EQUAL(response.first, "751");
	ASSERT(!response.second);
}


void TestRoutes()
{
	TestRunner tr;

	RUN_TEST(tr, TestReadRouteFromString);
	RUN_TEST(tr, TestRoutesDataBase);
}