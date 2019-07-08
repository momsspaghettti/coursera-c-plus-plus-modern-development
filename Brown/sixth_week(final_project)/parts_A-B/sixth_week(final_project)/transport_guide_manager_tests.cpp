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

		const std::string expected = "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length, 1 curvature\n"
			"Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length, 1 curvature\n"
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

		const std::string expected = "Bus wUZPglHbh: 3 stops on route, 2 unique stops, 16921.2 route length, 1 curvature\n"
			"Bus lw5PH6 Ul3: 3 stops on route, 2 unique stops, 16921.2 route length, 1 curvature\n"
			"Bus gazZviu ncDtm: 3 stops on route, 2 unique stops, 16921.2 route length, 1 curvature\n"
			"Bus gazZviu ncDtm: 3 stops on route, 2 unique stops, 16921.2 route length, 1 curvature\n"
			"Bus lw5PH6 Ul3: 3 stops on route, 2 unique stops, 16921.2 route length, 1 curvature\n"
			"Bus wUZPglHbh: 3 stops on route, 2 unique stops, 16921.2 route length, 1 curvature\n"
			"Bus gazZviu ncDtm: 3 stops on route, 2 unique stops, 16921.2 route length, 1 curvature\n"
			"Bus gazZviu ncDtm: 3 stops on route, 2 unique stops, 16921.2 route length, 1 curvature\n"
			"Bus MYb6ycMmNaxXe: not found\n"
			"Bus wUZPglHbh: 3 stops on route, 2 unique stops, 16921.2 route length, 1 curvature\n";

		ASSERT_EQUAL(output.str(), expected);
	}

	{
		TransportGuideManager manager;

		std::stringstream input, output;
		input << "40\n Bus Q: 6DlTRiDlBADHd5wxuGwM - hXCtNJ55eAcJyF - kv8A5AKGj iN - aaDrzCkRa - rOdgQuDph - " <<
			"mq35 - L5pcYa50O j4etgtHGQt - tPniDCwlR - RZIBvXF6op2sX5cURx - Mzx - pCumWQzxe 4xL4eWNY - " <<
			"XiIL1rrRfmLsCvs4t1 C4AzoT - G3o0bW - 9W6KH8pazOTFPc3zss7E3 - g0xPM - XB5NAUcKh yDNGkD7EtQ8 - " <<
			"bdIWPK1kWMN7wvJ7zl8Cgt7m - aiAnA4AhBRojwfcFz - hhmpyF gEZCyMurPa - XEQUDl69zPpE9 - SxvGUp9OhePe4pW FQfn" <<
			" - L5pcYa50O j4etgtHGQt - W93vQSGbE2mnskhufqwc - K - tQVD77CEYItf6vMk279fDk - 7hSQl1n2m - WUobJ5iHPZ974O " <<
			"- f4mV - XiIL1rrRfmLsCvs4t1 C4AzoT - 2IuMsuRbGaGlT - 4xyPd - PCKCn - mOA - 4pa5EXOMn4kJeA - gdhjubQAp3ZEgHbYdb " <<
			"- 9uyCGpPMbWiXVAwsXG - QOW7FWL5v5kdJBk3RDCZg - K - 6DlTRiDlBADHd5wxuGwM\n" <<
			"Stop 6DlTRiDlBADHd5wxuGwM: 1.1, -1.1\nStop hXCtNJ55eAcJyF: 1.1, -1.1\nStop kv8A5AKGj iN: 1.1, -1.1\n" <<
			"Stop aaDrzCkRa: 1.1, -1.1\nStop rOdgQuDph: 1.1, -1.1\nStop mq35: 1.1, -1.1\nStop L5pcYa50O j4etgtHGQt: 1.1, -1.1\n" <<
			"Stop tPniDCwlR: 1.1, -1.1\nStop RZIBvXF6op2sX5cURx: 1.1, -1.1\nStop Mzx: 1.1, -1.1\nStop pCumWQzxe 4xL4eWNY: 1.1, -1.1\n" <<
			"Stop XiIL1rrRfmLsCvs4t1 C4AzoT: 1.1, -1.1\nStop G3o0bW: 1.1, -1.1\nStop 9W6KH8pazOTFPc3zss7E3: 1.1, -1.1\n" <<
			"Stop g0xPM: 1.1, -1.1\nStop XB5NAUcKh yDNGkD7EtQ8: 1.1, -1.1\nStop bdIWPK1kWMN7wvJ7zl8Cgt7m: 1.1, -1.1\n" <<
			"Stop aiAnA4AhBRojwfcFz: 1.1, -1.1\nStop hhmpyF gEZCyMurPa: 1.1, -1.1\nStop XEQUDl69zPpE9: 1.1, -1.1\n" <<
			"Stop SxvGUp9OhePe4pW FQfn: 1.1, -1.1\nStop L5pcYa50O j4etgtHGQt: 1.1, -1.1\nStop W93vQSGbE2mnskhufqwc: 1.1, -1.1\n" <<
			"Stop K: 1.1, -1.1\nStop tQVD77CEYItf6vMk279fDk: 1.1, -1.1\nStop 7hSQl1n2m: 1.1, -1.1\nStop WUobJ5iHPZ974O: 1.1, -1.1\n" <<
			"Stop f4mV: 1.1, -1.1\nStop XiIL1rrRfmLsCvs4t1 C4AzoT: 1.1, -1.1\nStop 2IuMsuRbGaGlT: 1.1, -1.1\nStop 4xyPd: 1.1, -1.1\n" <<
			"Stop PCKCn: 1.1, -1.1\nStop mOA: 1.1, -1.1\nStop 4pa5EXOMn4kJeA: 1.1, -1.1\nStop gdhjubQAp3ZEgHbYdb: 1.1, -1.1\n" <<
			"Stop 9uyCGpPMbWiXVAwsXG: 1.1, -1.1\nStop QOW7FWL5v5kdJBk3RDCZg: 1.1, -1.1\nStop K: 1.1, 1.1\nStop 6DlTRiDlBADHd5wxuGwM: 1.1, -1.1\n" <<
			"1\nBus Q\n";

		manager.PerformQueries(input, output);
	}

	{
		TransportGuideManager manager;

		std::stringstream input, output;
		input << "13\nStop Tolstopaltsevo: 55.611087, 37.20829\n" <<
			"Stop Marushkino: 55.595884, 37.209755\nBus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > " <<
			"Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n" <<
			"Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n" <<
			"Stop Rasskazovka: 55.632761, 37.333324\nStop Biryulyovo Zapadnoye: 55.574371, 37.6517\n" <<
			"Stop Biryusinka: 55.581065, 37.64839\nStop Universam: 55.587655, 37.645687\n" <<
			"Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\nStop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n" <<
			"Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n" <<
			"Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\nStop Prazhskaya: 55.611678, 37.603831\n" <<
			"6\nBus 256\nBus 750\nBus 751\nStop Samara\nStop Prazhskaya\nStop Biryulyovo Zapadnoye\n";

		manager.PerformQueries(input, output);

		const std::string expected = "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length, 1 curvature\n"
			"Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length, 1 curvature\n"
			"Bus 751: not found\n"
			"Stop Samara: not found\n"
			"Stop Prazhskaya: no buses\n"
			"Stop Biryulyovo Zapadnoye: buses 256 828 \n";

		ASSERT_EQUAL(output.str(), expected);
	}

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
}


void TestTransportGuideManager()
{
	TestRunner tr;

	RUN_TEST(tr, TestTransportGuideManagerPerformQueries);
}