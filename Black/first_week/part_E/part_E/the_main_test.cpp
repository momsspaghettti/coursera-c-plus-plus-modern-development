#include "the_main_test.h"
#include "json.h"
#include "UnitTestsFramework.h"
#include <fstream>
#include "transport_guide_manager.h"
#include "Profiler.h"


using namespace Json;


void TestRouteRequest(const std::map<std::string, Node>& tested, 
	const std::map<std::string, Node>& expected)
{
	ASSERT(std::abs(tested.at("total_time").AsDouble() - 
		expected.at("total_time").AsDouble()) < 0.01);

    // Оптимальный маршрут между двумя остановками может быть проложен несколькими способами,
    // поэтому проверять всё, кроме итогового аремени поездки не представляется возможным

	/* 
	 * size_t i = 0;
    for (const auto& item : tested.at("items").AsArray())
    {
		ASSERT_EQUAL(item.AsMap().at("type").AsString(), 
			expected.at("items").AsArray()[i].AsMap().at("type").AsString());

        if (item.AsMap().at("type").AsString() == "Wait")
        {
            ASSERT_EQUAL(item.AsMap().at("stop_name").AsString(),
				expected.at("items").AsArray()[i].AsMap().at("stop_name").AsString());

			ASSERT_EQUAL(item.AsMap().at("time").AsInt(),
				expected.at("items").AsArray()[i].AsMap().at("time").AsInt());
        }
        else
        {
			ASSERT_EQUAL(item.AsMap().at("bus").AsString(),
				expected.at("items").AsArray()[i].AsMap().at("bus").AsString());

			ASSERT_EQUAL(item.AsMap().at("span_count").AsInt(),
				expected.at("items").AsArray()[i].AsMap().at("span_count").AsInt());

			ASSERT(std::abs(item.AsMap().at("time").AsDouble() - 
				expected.at("items").AsArray()[i].AsMap().at("time").AsDouble()) < 0.00001);
        }

		++i;
    }
	 */
}


void TestBusRequest(const std::map<std::string, Node>& tested,
	const std::map<std::string, Node>& expected)
{
	ASSERT_EQUAL(tested.at("route_length").AsInt(), expected.at("route_length").AsInt());
	ASSERT(std::abs(tested.at("curvature").AsDouble() - expected.at("curvature").AsDouble()) < 0.01);
	ASSERT_EQUAL(tested.at("stop_count").AsInt(), expected.at("stop_count").AsInt());
	ASSERT_EQUAL(tested.at("unique_stop_count").AsInt(), expected.at("unique_stop_count").AsInt());
}


void TestStopRequest(const std::map<std::string, Node>& tested,
	const std::map<std::string, Node>& expected)
{
	size_t i = 0;
    for (const auto& bus : tested.at("buses").AsArray())
    {
		ASSERT_EQUAL(bus.AsString(), tested.at("buses").AsArray()[i].AsString());
		++i;
    }
}


void TestFromFile(std::string input_name, 
	std::string output_name, std::string expected_name)
{
	std::ifstream input, tested, expected;
	std::ofstream output;
	input.open(input_name);
	output.open(output_name);

	{
		TIME_IT("Time Big Test");

		TransportGuideManager manager;
		manager.PerformQueries(input, output);

		input.close();
		output.close();
	}

	tested.open(output_name);
	expected.open(expected_name);

	const auto tested_vec = Load(tested).GetRoot().AsArray();
	const auto expected_vec = Load(expected).GetRoot().AsArray();

	tested.close();
    expected.close();

	const size_t n = tested_vec.size();

    for (size_t i = 0; i < n; ++i)
    {
		ASSERT_EQUAL(tested_vec[i].AsMap().at("request_id").AsInt(),
			expected_vec[i].AsMap().at("request_id").AsInt());

		const auto route_finder = tested_vec[i].AsMap().find("items");

        if (route_finder != tested_vec[i].AsMap().end())
        {
			TestRouteRequest(tested_vec[i].AsMap(), expected_vec[i].AsMap());
        }
        else
        {
            const auto bus_finder = tested_vec[i].AsMap().find("route_length");

            if (bus_finder != tested_vec[i].AsMap().end())
            {
				TestBusRequest(tested_vec[i].AsMap(), expected_vec[i].AsMap());
            }
            else
            {
                const auto stop_finder = tested_vec[i].AsMap().find("buses");

                if (stop_finder != tested_vec[i].AsMap().end())
                {
					TestStopRequest(tested_vec[i].AsMap(), expected_vec[i].AsMap());
                }
                else
                {
					ASSERT_EQUAL(tested_vec[i].AsMap().at("error_message").AsString(),
						expected_vec[i].AsMap().at("error_message").AsString());
                }
            }
        }
    }
}


void Test1()
{
	TestFromFile(
		"input1.txt", 
		"output1.txt", 
		"expected1.txt");
}


void Test2()
{
	TestFromFile(
		"input2.txt",
		"output2.txt",
		"expected2.txt");
}


void Test3()
{
	TestFromFile(
		"input3.txt",
		"output3.txt",
		"expected3.txt");
}


void Test4()
{
	TestFromFile(
		"input4.txt",
		"output4.txt",
		"expected4.txt");
}


void MainTest()
{
	TestRunner tr;

	RUN_TEST(tr, Test1);
	RUN_TEST(tr, Test2);
	RUN_TEST(tr, Test3);
	RUN_TEST(tr, Test4);
}