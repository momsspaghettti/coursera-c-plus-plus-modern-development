#include "the_main_test.h"
#include "json.h"
#include "UnitTestsFramework.h"
#include <fstream>


using namespace Json;


void TestRouteRequest(const std::map<std::string, Node>& tested, 
	const std::map<std::string, Node>& expected)
{
	ASSERT(std::abs(tested.at("total_time").AsDouble() - 
		expected.at("total_time").AsDouble()) < 0.00001);

	size_t i = 0;
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
}


void TestBusRequest(const std::map<std::string, Node>& tested,
	const std::map<std::string, Node>& expected)
{
	ASSERT_EQUAL(tested.at("route_length").AsInt(), expected.at("route_length").AsInt());
	ASSERT(std::abs(tested.at("curvature").AsDouble() - expected.at("curvature").AsDouble()) < 0.00001);
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


void TestBigData()
{
	std::ifstream input, expected;
	input.open("test_output.txt");
	expected.open("test_expected.txt");

	const auto input_vec = Load(input).GetRoot().AsArray();
	const auto expected_vec = Load(expected).GetRoot().AsArray();

	input.close();
	expected.close();

	const size_t n = input_vec.size();

    for (size_t i = 0; i < n; ++i)
    {
		ASSERT_EQUAL(input_vec[i].AsMap().at("request_id").AsInt(), 
			expected_vec[i].AsMap().at("request_id").AsInt());

		const auto route_finder = input_vec[i].AsMap().find("items");

        if (route_finder != input_vec[i].AsMap().end())
        {
			TestRouteRequest(input_vec[i].AsMap(), expected_vec[i].AsMap());
        }
        else
        {
            const auto bus_finder = input_vec[i].AsMap().find("route_length");

            if (bus_finder != input_vec[i].AsMap().end())
            {
				TestBusRequest(input_vec[i].AsMap(), expected_vec[i].AsMap());
            }
            else
            {
                const auto stop_finder = input_vec[i].AsMap().find("buses");

                if (stop_finder != input_vec[i].AsMap().end())
                {
					TestStopRequest(input_vec[i].AsMap(), expected_vec[i].AsMap());
                }
                else
                {
					ASSERT_EQUAL(input_vec[i].AsMap().at("error_message").AsString(), 
						expected_vec[i].AsMap().at("error_message").AsString());
                }
            }
        }
    }
}


void MainTest()
{
	TestRunner tr;

	RUN_TEST(tr, TestBigData);
}