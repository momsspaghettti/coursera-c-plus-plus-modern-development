#include "transport_guide_manager_tests.h"
#include "UnitTestsFramework.h"
#include "transport_guide_manager.h"


void TestTransportGuideManagerPerformQueries()
{
	{
		TransportGuideManager manager;

		std::stringstream input, output;
		input << "13\nStop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino\n" <<
			"Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka\n" <<
			"Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > " <<
	        "Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n" <<
			"Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n" <<
			"Stop Rasskazovka: 55.632761, 37.333324\n" <<
			"Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m " << 
			"to Biryusinka, 2400m to Universam\n" <<
			"Stop Biryusinka: 55.581065, 37.64839, 750m to Universam\n" <<
			"Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya\n" <<
			"Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya\n" << 
			"Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye\n" <<
			"Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n" <<
			"Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n" << 
			"Stop Prazhskaya: 55.611678, 37.603831\n" <<
			"6\nBus 256\nBus 750\nBus 751\nStop Samara\nStop Prazhskaya\nStop Biryulyovo Zapadnoye\n";

		manager.PerformQueries(input, output);

		const std::string expected = "Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.36124 curvature\n"
	        "Bus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.31808 curvature\n"
	        "Bus 751: not found\nStop Samara: not found\nStop Prazhskaya: no buses\n"
	        "Stop Biryulyovo Zapadnoye: buses 256 828 \n";

		ASSERT_EQUAL(output.str(), expected);
	}

	{
		TransportGuideManager manager;

		std::stringstream input, output;
		input << "2\nStop F xQ5MMcddWB04CuC83q4: 43.53887, 39.671003, 787891m to "
			"ze X7qQ60fJJ9t1mlo, 807966m to QE0QWM8c, 781301m to Xty mgg5zUNhVEcf, 434235m to "
			"gS0yXkXzsftitLYrb, 538102m to TPF8aq87Q60tnjHvJeqmnu5\n"
	        "Stop Mn9eeb: 43.501553, 39.883977, 737215m to p1KcKgCNVztH4ucLjy\n";

		manager.PerformQueries(input, output);
	}
}


void TestTransportGuideManager()
{
	TestRunner tr;

	RUN_TEST(tr, TestTransportGuideManagerPerformQueries);
}