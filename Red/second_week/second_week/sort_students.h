#pragma once
#include <map>
#include <string>


struct Student 
{
    std::string first_name;
    std::string last_name;
    std::map<std::string, double> marks;
    double rating;

    bool operator < (const Student& other) const 
    {
        return GetName() < other.GetName();
    }

    bool Less(const Student& other) const 
    {
        return rating > other.rating;
    }

    std::string GetName() const 
    {
      return first_name + " " + last_name;
    }
};


void TestSortStudents();