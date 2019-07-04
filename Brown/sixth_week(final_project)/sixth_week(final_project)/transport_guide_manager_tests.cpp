#include "transport_guide_manager_tests.h"
#include "UnitTestsFramework.h"
#include "transport_guide_manager.h"


void TestTransportGuideManagerPerformQueries()
{
	{
		TransportGuideManager manager;

		std::stringstream input, output;
		input << "10\nStop Tolstopaltsevo: 55.611087, 37.20829\n" <<
			"Stop Marushkino: 55.595884, 37.209755\n" <<
			"Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > " <<
			"Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n" <<
			"Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n" <<
			"Stop Rasskazovka: 55.632761, 37.333324\n" <<
			"Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n" <<
			"Stop Biryusinka: 55.581065, 37.64839\n" <<
			"Stop Universam: 55.587655, 37.645687\n" <<
			"Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n" <<
			"Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n" <<
			"3\nBus 256\nBus 750\nBus 751\n";

		manager.PerformQueries(input, output);

		const std::string expected = "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
			"Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
			"Bus 751: not found\n";

		ASSERT_EQUAL(output.str(), expected);
	}

	{
	    TransportGuideManager manager;

		std::stringstream input, output;
		input << "5\nBus gazZviu ncDtm: C06g9m0rff9 > 9CBIn uqPIOjG > C06g9m0rff9\n" <<
			"Stop C06g9m0rff9: 43.626432, 39.917507\n" <<
			"Bus lw5PH6 Ul3: C06g9m0rff9 - 9CBIn uqPIOjG\n" <<
			"Bus wUZPglHbh: C06g9m0rff9 - 9CBIn uqPIOjG\n" <<
			"Stop 9CBIn uqPIOjG: 43.634987, 39.813051\n" <<
			"10\nBus wUZPglHbh\nBus lw5PH6 Ul3\nBus gazZviu ncDtm\n" <<
			"Bus gazZviu ncDtm\nBus lw5PH6 Ul3\nBus wUZPglHbh\nBus gazZviu ncDtm\n" <<
			"Bus gazZviu ncDtm\nBus MYb6ycMmNaxXe\nBus wUZPglHbh\n";

		manager.PerformQueries(input, output);

		const std::string expected = "Bus wUZPglHbh: 3 stops on route, 2 unique stops, 16921.2 route length\n"
			"Bus lw5PH6 Ul3: 3 stops on route, 2 unique stops, 16921.2 route length\n"
			"Bus gazZviu ncDtm: 3 stops on route, 2 unique stops, 16921.2 route length\n"
			"Bus gazZviu ncDtm: 3 stops on route, 2 unique stops, 16921.2 route length\n"
			"Bus lw5PH6 Ul3: 3 stops on route, 2 unique stops, 16921.2 route length\n"
			"Bus wUZPglHbh: 3 stops on route, 2 unique stops, 16921.2 route length\n"
			"Bus gazZviu ncDtm: 3 stops on route, 2 unique stops, 16921.2 route length\n"
			"Bus gazZviu ncDtm: 3 stops on route, 2 unique stops, 16921.2 route length\n"
			"Bus MYb6ycMmNaxXe: not found\n"
			"Bus wUZPglHbh: 3 stops on route, 2 unique stops, 16921.2 route length\n";

		ASSERT_EQUAL(output.str(), expected);
	}
}


void TestTransportGuideManager()
{
	TestRunner tr;

	RUN_TEST(tr, TestTransportGuideManagerPerformQueries);
}