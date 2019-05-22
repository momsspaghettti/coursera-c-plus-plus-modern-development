#pragma once
#include <string>
#include <map>


std::string FindNameByYear(const std::map<int, std::string>& names, int year);


class Person
{
public:
	void ChangeFirstName(int year, const std::string& first_name);

	void ChangeLastName(int year, const std::string& last_name);

    std::string GetFullName(int year);

private:
	std::map<int, std::string> first_names;
	std::map<int, std::string> last_names;
};

