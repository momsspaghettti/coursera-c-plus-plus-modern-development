#pragma once
#include "json.h"
#include "spending.h"


inline std::vector<Spending> LoadFromJson(std::istream& input)
{
	const std::vector<Json::Node> root = Json::Load(input).GetRoot().AsArray();

	std::vector<Spending> result;
	result.reserve(root.size());

    for (const Json::Node& child : root)
    {
		result.push_back(Spending{ child.AsMap().at("category").AsString(), 
			child.AsMap().at("amount").AsInt() });
    }

	return result;
}


void TestSpendingsJSON();