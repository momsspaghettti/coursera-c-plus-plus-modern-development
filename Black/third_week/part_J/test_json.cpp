#include "test_json.h"
#include "UnitTestsFramework.h"
#include "json.h"
#include <sstream>


void TestLoadJson()
{
	using namespace Json;

	{
		std::stringstream input("  {"
			"\"type\":   \"Stop\","
			"\"road_distances\" :  {"
			"\"Marushkino\":   3900"
		"},"
			"\"longitude\"  :  37.20829 ,"
				"\"name\" : \"Tolstopaltsevo\"  , "
				"\"latitude\" :   55.611087"
	    "  }\"");

		const Document doc = Load(input);

		auto root_map = doc.GetRoot().AsMap();

		ASSERT_EQUAL(root_map["type"].AsString(), "Stop");
		ASSERT_EQUAL(root_map["road_distances"].AsMap().at("Marushkino").AsInt(), 3900);
		ASSERT(std::abs(root_map["longitude"].AsDouble() - 37.20829) < 0.0000001);
		ASSERT_EQUAL(root_map["name"].AsString(), "Tolstopaltsevo");
		ASSERT(std::abs(root_map["latitude"].AsDouble() - 55.611087) < 0.0000001);
	}

	{
		std::stringstream input("  {"
			"\"type\":   \"Bus\","
			"\"stops\" :  ["
			"\"Biryulyovo Zapadnoye\","
			"\"Biryusinka\","
			"\"Universam\","
			"\"Biryulyovo Tovarnaya\","
			"\"Biryulyovo Passazhirskaya\","
			"\"Biryulyovo Zapadnoye\"        "
	  "     ], "
            "\"is_roundtrip\": true"
			"\"name\" : \"256\"   "
			"  }\"");

		const Document doc = Load(input);

		auto root_map = doc.GetRoot().AsMap();

		const std::vector<std::string> expected_stops = { "Biryulyovo Zapadnoye", "Biryusinka", "Universam", 
		    "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"};

		ASSERT_EQUAL(root_map["type"].AsString(), "Bus");
		ASSERT_EQUAL(root_map["name"].AsString(), "256");
		ASSERT(root_map["is_roundtrip"].AsBool());
        for (size_t i = 0; i < 6; ++i)
        {
			ASSERT_EQUAL(root_map["stops"].AsArray()[i].AsString(), expected_stops[i]);
        }
	}

	{
		std::stringstream input("  {"
			"\"type\":   \"Stop\","
			"\"road_distances\" :  {"
			"\"Rossoshanskaya ulitsa\":   7500  ,"
            "\"Biryusinka\":  1800 , "
            "\"Universam\"   :2400"
			"},"
			"\"longitude\"  :  37.6517 ,"
			"\"name\" : \"Biryulyovo Zapadnoye\"  , "
			"\"latitude\":55.574371"
			"  }\"");

		const Document doc = Load(input);

		auto root_map = doc.GetRoot().AsMap();

		ASSERT_EQUAL(root_map["type"].AsString(), "Stop");
		ASSERT(std::abs(root_map["longitude"].AsDouble() - 37.6517) < 0.0000001);
		ASSERT_EQUAL(root_map["name"].AsString(), "Biryulyovo Zapadnoye");
		ASSERT(std::abs(root_map["latitude"].AsDouble() - 55.574371) < 0.0000001);

		const std::map<std::string, unsigned> expected_dist = { {"Biryusinka", 1800},
			{"Rossoshanskaya ulitsa", 7500}, {"Universam", 2400} };
        for (const auto& p : root_map["road_distances"].AsMap())
        {
			ASSERT_EQUAL(p.second.AsInt(), expected_dist.at(p.first));
        }
	}

	{
		std::stringstream input("  {"
			"\"type\":   \"Bus\","
			"\"stops\" :  ["
			"\"Biryulyovo Zapadnoye\","
			"\"Universam\","
			"\"Rossoshanskaya ulitsa\","
			"\"Biryulyovo Zapadnoye\"        "
			"     ], "
			"\"is_roundtrip\": true"
			"\"name\" : \"828\"   "
			"  }\"");

		const Document doc = Load(input);

		auto root_map = doc.GetRoot().AsMap();

		const std::vector<std::string> expected_stops = { "Biryulyovo Zapadnoye", "Universam",
			"Rossoshanskaya ulitsa", "Biryulyovo Zapadnoye" };

		ASSERT_EQUAL(root_map["type"].AsString(), "Bus");
		ASSERT_EQUAL(root_map["name"].AsString(), "828");
		ASSERT(root_map["is_roundtrip"].AsBool());
		for (size_t i = 0; i < 4; ++i)
		{
			ASSERT_EQUAL(root_map["stops"].AsArray()[i].AsString(), expected_stops[i]);
		}
	}

	{
		std::stringstream input("  {"
			"\"type\":   \"Bus\","
			"\"stops\" :  ["
			"\"Biryulyovo Zapadnoye\","
			"\"Universam\","
			"\"Rossoshanskaya ulitsa\","
			"\"Biryulyovo Zapadnoye\"        "
			"     ], "
			"\"is_roundtrip\": false"
			"\"name\" : \"828\"   "
			"  }\"");

		const Document doc = Load(input);

		auto root_map = doc.GetRoot().AsMap();

		const std::vector<std::string> expected_stops = { "Biryulyovo Zapadnoye", "Universam",
			"Rossoshanskaya ulitsa", "Biryulyovo Zapadnoye" };

		ASSERT_EQUAL(root_map["type"].AsString(), "Bus");
		ASSERT_EQUAL(root_map["name"].AsString(), "828");
		ASSERT(!root_map["is_roundtrip"].AsBool());
		for (size_t i = 0; i < 4; ++i)
		{
			ASSERT_EQUAL(root_map["stops"].AsArray()[i].AsString(), expected_stops[i]);
		}
	}

	{
		std::stringstream input("  {"
			"\"type\":   \"Stop\","
			"\"road_distances\" :  {       },"
			"\"longitude\"  :  37.6517 ,"
			"\"name\" : \"Biryulyovo Zapadnoye\"  , "
			"\"latitude\":55.574371"
			"  }\"");

		const Document doc = Load(input);

		auto root_map = doc.GetRoot().AsMap();

		ASSERT_EQUAL(root_map["type"].AsString(), "Stop");
		ASSERT(std::abs(root_map["longitude"].AsDouble() - 37.6517) < 0.0000001);
		ASSERT_EQUAL(root_map["name"].AsString(), "Biryulyovo Zapadnoye");
		ASSERT(std::abs(root_map["latitude"].AsDouble() - 55.574371) < 0.0000001);
		ASSERT(root_map["road_distances"].AsMap().empty());
	}

	{
		std::stringstream input("  {"
			"\"type\":   \"Stop\","
			"\"road_distances\" :  {       },"
			"\"longitude\"  :  30 ,"
			"\"name\" : \"Biryulyovo Zapadnoye\"  , "
			"\"latitude\":-55"
			"  }\"");

		const Document doc = Load(input);

		auto root_map = doc.GetRoot().AsMap();

		ASSERT_EQUAL(root_map["type"].AsString(), "Stop");
		ASSERT(std::abs(root_map["longitude"].AsDouble() - 30) < 0.0000001);
		ASSERT_EQUAL(root_map["name"].AsString(), "Biryulyovo Zapadnoye");
		ASSERT(std::abs(root_map["latitude"].AsDouble() - (-55)) < 0.0000001);
		ASSERT(root_map["road_distances"].AsMap().empty());
	}

	{
		std::stringstream input("  {"
			"\"type\":   \"Stop\",   "
			"  \"road_distances\" :  {       },"
			"    \"longitude\"  :  -37.6517                        ,"
			"\"name\":\"Biryulyovo Zapadnoye\"  , "
			"\"latitude\":-55.574371      "
			"  }         \"");

		const Document doc = Load(input);

		auto root_map = doc.GetRoot().AsMap();

		ASSERT_EQUAL(root_map["type"].AsString(), "Stop");
		ASSERT(std::abs(root_map["longitude"].AsDouble() - (-37.6517)) < 0.0000001);
		ASSERT_EQUAL(root_map["name"].AsString(), "Biryulyovo Zapadnoye");
		ASSERT(std::abs(root_map["latitude"].AsDouble() - (-55.574371)) < 0.0000001);
		ASSERT(root_map["road_distances"].AsMap().empty());
	}
}


void TestJsonWrite()
{
	using namespace Json;

	{
		std::vector<std::shared_ptr<JsonWriter>> test;

		for (int i = 0; i < 5; ++i)
		{
			test.push_back(
				std::make_shared<JsonInt>(i));
		}

		const std::string expected = "[ 0, 1, 2, 3, 4 ]";
		ASSERT_EQUAL(JsonArray(test).ToString(), expected);
	}

	{
		std::map<std::string, std::shared_ptr<JsonWriter>> test_dict;

		test_dict["type"] = std::make_shared<JsonString>("Bus");
        test_dict["name"] = std::make_shared<JsonString>("256");

		std::vector<std::shared_ptr<JsonWriter>> test_array;
        test_array.push_back(
			std::make_shared<JsonString>("Biryulyovo Zapadnoye"));
		test_array.push_back(
			std::make_shared<JsonString>("Biryusinka"));
		test_array.push_back(
			std::make_shared<JsonString>("Universam"));
		test_array.push_back(
			std::make_shared<JsonString>("Biryulyovo Tovarnaya"));
		test_array.push_back(
			std::make_shared<JsonString>("Biryulyovo Passazhirskaya"));
		test_array.push_back(
			std::make_shared<JsonString>("Biryulyovo Zapadnoye"));

		test_dict["stops"] = std::make_shared<JsonArray>(test_array);
		test_dict["is_roundtrip"] = std::make_shared<JsonBool>(true);

		const std::string expected = "{ \"is_roundtrip\": true, \"name\": \"256\", \"stops\": "
	        "[ \"Biryulyovo Zapadnoye\", \"Biryusinka\", \"Universam\", \"Biryulyovo Tovarnaya\", "
	        "\"Biryulyovo Passazhirskaya\", \"Biryulyovo Zapadnoye\" ], \"type\": \"Bus\" }";

		ASSERT_EQUAL(JsonMap(test_dict).ToString(), expected);
	}

	{
		std::vector<std::shared_ptr<JsonWriter>> test_array;

		std::map<std::string, std::shared_ptr<JsonWriter>> test_dict1, test_dict2;

		test_dict1["route_length"] = std::make_shared<JsonInt>(5950);
		test_dict1["request_id"] = std::make_shared<JsonInt>(1965312327);
        test_dict1["curvature"] = std::make_shared<JsonDouble>(1.36124);
		test_dict1["stop_count"] = std::make_shared<JsonInt>(6);
		test_dict1["unique_stop_count"] = std::make_shared<JsonInt>(5);

		test_dict2["request_id"] = std::make_shared<JsonInt>(194217464);
		test_dict2["error_message"] = std::make_shared<JsonString>("not found");

		test_array.push_back(std::make_shared<JsonMap>(test_dict1));
		test_array.push_back(std::make_shared<JsonMap>(test_dict2));

		const std::string expected = "[ { \"curvature\": 1.36124, \"request_id\": 1965312327, "
	        "\"route_length\": 5950, \"stop_count\": 6, \"unique_stop_count\": 5 }, { \"error_message\": \"not found\", "
	        "\"request_id\": 194217464 } ]";

		ASSERT_EQUAL(JsonArray(test_array).ToString(), expected);
	}
}


void TestJson()
{
	TestRunner tr;

	RUN_TEST(tr, TestLoadJson);
	RUN_TEST(tr, TestJsonWrite);
}