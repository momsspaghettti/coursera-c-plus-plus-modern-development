#pragma once
#include "xml.h"
#include "spending.h"


inline std::vector<Spending> LoadFromXml(std::istream& input)
{
	const Xml::Node root = Xml::Load(input).GetRoot();

	std::vector<Spending> result;
	result.reserve(root.Children().size());

    for (const Xml::Node& child : root.Children())
    {
		result.push_back(Spending{ child.AttributeValue<std::string>("category"), 
			child.AttributeValue<int>("amount") });
    }

	return result;
}


void TestSpendingsXML();