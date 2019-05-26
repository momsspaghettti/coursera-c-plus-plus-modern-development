#include "sort_students.h"
#include "UnitTestsFramework.h"
#include <algorithm>
#include "Profiler.h"


bool Compare(const Student& first, const Student& second) 
{
	return first.rating > second.rating;
}


void TestComparison() 
{
    Student newbie {
        "Ivan", "Ivanov", {
        {"c++", 1.0},
        {"algorithms", 3.0}},
        2.0};
    
    Student cpp_expert {
        "Petr", "Petrov", {
        {"c++", 9.5},
        {"algorithms", 6.0}},
        7.75};

    Student guru {
        "Sidor", "Sidorov", {
        {"c++", 10.0},
        {"algorithms", 10.0}},
        10.0
    };
    
    ASSERT(Compare(guru, newbie));
    ASSERT(Compare(guru, cpp_expert));
    ASSERT(!Compare(newbie, cpp_expert));
}


void TestSorting() 
{
    std::vector<Student> students {
        {"Sidor", "Sidorov", {{"maths", 2.}}, 2.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
        {"Semen", "Semenov", {{"maths", 4.}}, 4.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
        {"Ivan", "Ivanov", {{"maths", 5.}}, 5.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
        {"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
        {"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
        {"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Sidor", "Sidorov", {{"maths", 2.}}, 2.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Semen", "Semenov", {{"maths", 4.}}, 4.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Ivan", "Ivanov", {{"maths", 5.}}, 5.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Sidor", "Sidorov", {{"maths", 2.}}, 2.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Sidor", "Sidorov", {{"maths", 2.}}, 2.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Semen", "Semenov", {{"maths", 4.}}, 4.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Ivan", "Ivanov", {{"maths", 5.}}, 5.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Sidor", "Sidorov", {{"maths", 2.}}, 2.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Semen", "Semenov", {{"maths", 4.}}, 4.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Ivan", "Ivanov", {{"maths", 5.}}, 5.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Semen", "Semenov", {{"maths", 4.}}, 4.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Ivan", "Ivanov", {{"maths", 5.}}, 5.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Sidor", "Sidorov", {{"maths", 2.}}, 2.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Sidor", "Sidorov", {{"maths", 2.}}, 2.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Semen", "Semenov", {{"maths", 4.}}, 4.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Ivan", "Ivanov", {{"maths", 5.}}, 5.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Sidor", "Sidorov", {{"maths", 2.}}, 2.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Semen", "Semenov", {{"maths", 4.}}, 4.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Ivan", "Ivanov", {{"maths", 5.}}, 5.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Semen", "Semenov", {{"maths", 4.}}, 4.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Ivan", "Ivanov", {{"maths", 5.}}, 5.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Petr", "Petrov", {{"maths", 3.}}, 3.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}}, 1.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.},
		{"Alexander", "Alexandrov", {{"maths", 1.}, {"c++", 10.0}, {"algorithms", 5.0}}, 8.}};
    
    sort(students.begin(), students.end(), Compare);
  
    ASSERT(is_sorted(students.begin(), students.end(),
		[](const Student first, const Student second) 
	    {
            return first.Less(second);
        }));
}


void TestSortStudents()
{
	TIME_IT("TestSortStudents time");
    TestRunner tr;
    RUN_TEST(tr, TestComparison);
    RUN_TEST(tr, TestSorting);
}