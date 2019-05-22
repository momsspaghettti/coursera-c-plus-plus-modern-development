#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "split_into_words.h"
#include <algorithm>
//#include "demographics.h"
#include <iterator>
#include <deque>
#include "Person.h"


template <typename T>
std::vector<T> FindGreaterElements(const std::set<T>& elements, const T& border);


template <typename T>
std::vector<T> FindGreaterElements(const std::set<T>& elements, const T& border)
{
	std::vector<T> result;

	for (auto it = elements.begin(); it != elements.end(); ++it)
	{
	    if (*it > border)
	    {
			result.push_back(*it);
	    }
	}

	return result;
}


template <typename T>
void RemoveDuplicates(std::vector<T>& elements);


template <typename T>
void RemoveDuplicates(std::vector<T>& elements)
{
	sort(elements.begin(), elements.end());

	auto it = elements.begin();
	it = std::unique(it, elements.end());

	elements.erase(it, elements.end());
}


void MakePermutations()
{
	int n;

	std::cin >> n;

	std::vector<int> result;

    for (int i = n; i >=1; --i)
    {
		result.push_back(i);
    }

    do
    {
        for (const int& item : result)
        {
			std::cout << item << " ";
        }
		std::cout << std::endl;
    }
    while (std::next_permutation(result.begin(), result.end(), 
		[](const int& left, const int& right)
                {
		return left > right;
                }));

	//getchar();
}


/*
template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
    if (range_end - range_begin < 2)
    {
        return;
    }

	std::vector<typename RandomIt::value_type> elements(range_begin, range_end);

	RandomIt range_middle = elements.begin();

    while (true)
    {
        if (range_middle - elements.begin() == elements.end() - range_middle)
        {
            break;
        }
        else
        {
			++range_middle;
        }
    }

	MergeSort(elements.begin(), range_middle);
	MergeSort(range_middle, elements.end());

	std::merge(elements.begin(), range_middle, range_middle, elements.end(), range_begin);
}
*/


template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
	if (range_end - range_begin < 2)
	{
		return;
	}

	std::vector<typename RandomIt::value_type> elements(range_begin, range_end);

	RandomIt range_first_third = elements.begin();

	while (true)
	{
		if ((range_first_third - elements.begin()) * 2 == (elements.end() - range_first_third))
		{
			break;
		}
		else
		{
			++range_first_third;
		}
	}

	RandomIt range_second_third = range_first_third;

	while (true)
	{
		if ((range_second_third - elements.begin()) == (elements.end() - range_second_third) * 2)
		{
			break;
		}
		else
		{
			++range_second_third;
		}
	}

	MergeSort(elements.begin(), range_first_third);
	MergeSort(range_first_third, range_second_third);
	MergeSort(range_second_third, elements.end());

	std::vector<typename RandomIt::value_type> tmp_vec;

	std::merge(elements.begin(), range_first_third, range_first_third, 
		range_second_third, std::back_inserter(tmp_vec));

	std::merge(tmp_vec.begin(), tmp_vec.end(), range_second_third,
		elements.end(), range_begin);
}


std::set<int>::const_iterator FindNearestElement(const std::set<int>& numbers, int border)
{
	auto right_iter = numbers.lower_bound(border);

    if (right_iter == numbers.begin())
    {
		return right_iter;
    }

	auto left_iter = std::prev(right_iter);

    if (right_iter == numbers.end())
    {
		return left_iter;
    }

    if (border - *left_iter <= *right_iter - border)
    {
		return left_iter;
    }
    else
    {
		return right_iter;
    }
}


template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix)
{
	std::string prefix_;
	prefix_[0] = prefix;

	RandomIt left_iter = std::lower_bound(range_begin, range_end, prefix_, 
		[](const std::string& l, const std::string& r)
	{
		return l[0] < r[0];
	});

	RandomIt right_iter = std::upper_bound(range_begin, range_end, prefix_,
		[](const std::string& l, const std::string& r)
	{
		return l[0] < r[0];
	});

	return std::make_pair(left_iter, right_iter);
}


bool lower_comparator(const std::string& left, const std::string& right)
{

    if (right.size() > left.size())
    {
		return left < right;
    }

	const size_t size = std::min(right.size(), left.size());

	return left.substr(0, size) < right.substr(0, size);
}


bool upper_comparator(const std::string& left, const std::string& right)
{

	if (left.size() > right.size())
	{
		return left < right;
	}

	const size_t size = std::min(right.size(), left.size());

	return left.substr(0, size) < right.substr(0, size);
}


template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, const std::string& prefix)
{
	RandomIt left_iter = std::lower_bound(range_begin, range_end, prefix, lower_comparator);

	RandomIt right_iter = std::upper_bound(range_begin, range_end, prefix, upper_comparator);

	return std::make_pair(left_iter, right_iter);
}


void MakeExpression()
{
	std::deque<std::string> expression;

	int x, n;

	std::cin >> x >> n;

	expression.push_back(std::to_string(x));

	std::string operation;

    for (int i = 0; i < n; ++i)
    {
		std::string sign;
		int number;
        std::cin >> sign >> number;

		operation = sign + " " + std::to_string(number);

		expression.push_front("(");
		expression.push_back(")");
		expression.push_back(" " + operation);
    }

    for (const std::string item : expression)
    {
		std::cout << item;
    }

	//getchar();
}


void MakeExpressionWithoutBrackets()
{
	std::map<std::string, int> operations_priority = { {"*", 1}, {"/", 1}, {"+", 0}, {"-", 0} };

	std::deque<std::string> expression;

	int x, n;

	std::cin >> x >> n;

	expression.push_back(std::to_string(x));

	std::string operation;
	std::string current_sign, previos_sign;

	for (int i = 0; i < n; ++i)
	{
		int number;
		previos_sign = current_sign;

		std::cin >> current_sign >> number;

		operation = current_sign + " " + std::to_string(number);

        if (!previos_sign.empty() && 
			operations_priority.at(current_sign) > operations_priority.at(previos_sign))
        {
			expression.push_front("(");
			expression.push_back(")");
        }

		expression.push_back(" " + operation);
	}

	for (const std::string item : expression)
	{
		std::cout << item;
	}

	//getchar();
}


int main()
{
	MakeExpressionWithoutBrackets();

	//getchar();

    return 0;
}
