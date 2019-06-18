#include "json.h"
#include "UnitTestsFramework.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "spendings_json.h"


void TestLoadFromJson()
{
	std::istringstream json_input(R"([
    {"amount": 2500, "category": "food"},
    {"amount": 1150, "category": "transport"},
    {"amount": 5780, "category": "restaurants"},
    {"amount": 7500, "category": "clothes"},
    {"amount": 23740, "category": "travel"},
    {"amount": 12000, "category": "sport"}
  ])");

	const std::vector<Spending> spendings = LoadFromJson(json_input);

	const std::vector<Spending> expected = {
	  {"food", 2500},
	  {"transport", 1150},
	  {"restaurants", 5780},
	  {"clothes", 7500},
	  {"travel", 23740},
	  {"sport", 12000}
	};
	ASSERT_EQUAL(spendings, expected);
}


void TestJsonLibrary() 
{
	using namespace Json;
	// Тест демонстрирует, как пользоваться библиотекой из файла json.h

	std::istringstream json_input(R"([
    {"amount": 2500, "category": "food"},
    {"amount": 1150, "category": "transport"},
    {"amount": 12000, "category": "sport"}
  ])");

    Document doc = Load(json_input);
	const std::vector<Node>& root = doc.GetRoot().AsArray();
	ASSERT_EQUAL(root.size(), 3u);

	const std::map<std::string, Node>& food = root.front().AsMap();
	ASSERT_EQUAL(food.at("category").AsString(), "food");
	ASSERT_EQUAL(food.at("amount").AsInt(), 2500);

	const std::map<std::string, Node>& sport = root.back().AsMap();
	ASSERT_EQUAL(sport.at("category").AsString(), "sport");
	ASSERT_EQUAL(sport.at("amount").AsInt(), 12000);

    Node transport(std::map<std::string, Node>{ {"category", Node("transport")}, 
		{ "amount", Node(1150) }});
    Node array_node(std::vector<Node>{transport});
	ASSERT_EQUAL(array_node.AsArray().size(), 1u);
}


void TestSpendingsJSON()
{
	TestRunner tr;
	RUN_TEST(tr, TestJsonLibrary);
	RUN_TEST(tr, TestLoadFromJson);
}