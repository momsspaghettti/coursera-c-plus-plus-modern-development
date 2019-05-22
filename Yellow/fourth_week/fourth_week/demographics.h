#pragma once
#include <vector>


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


void PrintStats(std::vector<Person> persons);
