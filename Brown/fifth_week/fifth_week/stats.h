#pragma once
#include <vector>
#include <algorithm>


enum class Gender 
{
	FEMALE,
	MALE
};


struct Person
{
	int age;  // возраст
	Gender gender;  // пол
	bool is_employed;  // имеет ли работу
};


template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end)
{
	if (range_begin == range_end)
	{
		return 0;
	}
	
    std::vector<typename InputIt::value_type> range_copy(range_begin, range_end);
	auto middle = begin(range_copy) + range_copy.size() / 2;
	std::nth_element(begin(range_copy), middle, end(range_copy), 
		[](const Person& lhs, const Person& rhs) 
	{
		return lhs.age < rhs.age;
	});
	
    return middle->age;
}


void PrintStats(std::vector<Person> persons);


void TestPrintStats();