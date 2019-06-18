#pragma once
#include <iostream>
#include <algorithm>


struct Spending
{
	std::string category;
	int amount;
};


inline bool operator==(const Spending& lhs, const Spending& rhs)
{
	return lhs.category == rhs.category && lhs.amount == rhs.amount;
}


inline std::ostream& operator<<(std::ostream& os, const Spending& s)
{
	return os << '(' << s.category << ": " << s.amount << ')';
}


inline int CalculateTotalSpendings(const std::vector<Spending>& spendings)
{
	int result = 0;
	for (const Spending& s : spendings)
	{
		result += s.amount;
	}
	return result;
}


inline std::string MostExpensiveCategory(const std::vector<Spending>& spendings)
{
	const auto compare_by_amount =
		[](const Spending& lhs, const Spending& rhs) {
		return lhs.amount < rhs.amount;
	};

	return std::max_element(begin(spendings), end(spendings),
		compare_by_amount)->category;
}