#include "stats.h"
#include "UnitTestsFramework.h"
#include <algorithm>
#include <string>


void PrintStats(std::vector<Person> persons)
{
	const std::string prefix = "Median age ";

	const auto gender_border = std::partition(persons.begin(), persons.end(), 
		[](const Person& p)
	{
		return p.gender == Gender::MALE;
	});

	const auto male_employ_border = std::partition(persons.begin(), gender_border,
		[](const Person& p)
	{
		return p.is_employed;
	});

	const auto female_employ_border = std::partition(gender_border, persons.end(),
		[](const Person& p)
	{
		return p.is_employed;
	});

	std::cout << prefix << "= " << ComputeMedianAge(persons.begin(), persons.end()) << '\n';
	std::cout << prefix << "for females = " << ComputeMedianAge(gender_border, persons.end()) << '\n';
	std::cout << prefix << "for males = " << ComputeMedianAge(persons.begin(), gender_border) << '\n';
	std::cout << prefix << "for employed females = " << ComputeMedianAge(gender_border, female_employ_border) << '\n';
	std::cout << prefix << "for unemployed females = " << ComputeMedianAge(female_employ_border, persons.end()) << '\n';
	std::cout << prefix << "for employed males = " << ComputeMedianAge(persons.begin(), male_employ_border) << '\n';
	std::cout << prefix << "for unemployed males = " << ComputeMedianAge(male_employ_border, gender_border) << '\n';
}


void TestPrintStats()
{
	std::vector<Person> persons = {
	  {31, Gender::MALE, false},
	  {40, Gender::FEMALE, true},
	  {24, Gender::MALE, true},
	  {20, Gender::FEMALE, true},
	  {80, Gender::FEMALE, false},
	  {78, Gender::MALE, false},
	  {10, Gender::FEMALE, false},
	  {55, Gender::MALE, true},
	};

	PrintStats(persons);
}