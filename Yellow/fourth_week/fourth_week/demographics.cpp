#include "demographics.h"
#include <iostream>
#include <algorithm>


template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
	if (range_begin == range_end) {
		return 0;
	}
	std::vector<typename InputIt::value_type> range_copy(range_begin, range_end);
	auto middle = begin(range_copy) + range_copy.size() / 2;
	nth_element(
		begin(range_copy), middle, end(range_copy),
		[](const Person& lhs, const Person& rhs) {
		return lhs.age < rhs.age;
	}
	);
	return middle->age;
}


void PrintStats(std::vector<Person> persons)
{
    std::cout << "Median age = " << 
		ComputeMedianAge(persons.begin(), persons.end()) << std::endl;


    const auto female_iter = std::partition(persons.begin(), persons.end(), 
		[](const Person& person)
    {
		return person.gender == Gender::FEMALE;
	});

	std::cout << "Median age for females = " << 
		ComputeMedianAge(persons.begin(), female_iter) << std::endl;


	const auto male_iter = std::partition(persons.begin(), persons.end(),
		[](const Person& person)
	{
		return person.gender == Gender::MALE;
	});

	std::cout << "Median age for males = " << 
		ComputeMedianAge(persons.begin(), male_iter) << std::endl;


    const auto employed_female_iter = std::partition(persons.begin(), persons.end(),
		[](const Person& person)
	{
		return person.gender == Gender::FEMALE && person.is_employed;
	});

	std::cout << "Median age for employed females = " << 
		ComputeMedianAge(persons.begin(), employed_female_iter) << std::endl;


	const auto unemployed_female_iter = std::partition(persons.begin(), persons.end(),
		[](const Person& person)
	{
		return person.gender == Gender::FEMALE && !person.is_employed;
	});

	std::cout << "Median age for unemployed females = " <<
		ComputeMedianAge(persons.begin(), unemployed_female_iter) << std::endl;


	const auto employed_male_iter = std::partition(persons.begin(), persons.end(),
		[](const Person& person)
	{
		return person.gender == Gender::MALE && person.is_employed;
	});

	std::cout << "Median age for employed males = " <<
		ComputeMedianAge(persons.begin(), employed_male_iter) << std::endl;


	const auto unemployed_male_iter = std::partition(persons.begin(), persons.end(),
		[](const Person& person)
	{
		return person.gender == Gender::MALE && !person.is_employed;
	});

	std::cout << "Median age for unemployed males = " <<
		ComputeMedianAge(persons.begin(), unemployed_male_iter) << std::endl;

	//getchar();
}