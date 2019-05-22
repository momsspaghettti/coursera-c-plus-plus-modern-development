#include "Person.h"
#include <iterator>


std::string FindNameByYear(const std::map<int, std::string>& names, int year)
{
	auto iter = names.upper_bound(year);

	if (iter != names.begin())
	{
		return std::prev(iter)->second;
	}

	return "";
}


void Person::ChangeFirstName(int year, const std::string& first_name)
{
	first_names[year] = first_name;
}


void Person::ChangeLastName(int year, const std::string& last_name)
{
	last_names[year] = last_name;
}


std::string Person::GetFullName(int year)
{
	// получаем имя и фамилию по состоянию на год year
	const std::string first_name = FindNameByYear(first_names, year);
	const std::string last_name = FindNameByYear(last_names, year);

	// если и имя, и фамилия неизвестны
	if (first_name.empty() && last_name.empty()) {
		return "Incognito";

		// если неизвестно только имя
	}
	else if (first_name.empty()) {
		return last_name + " with unknown first name";

		// если неизвестна только фамилия
	}
	else if (last_name.empty()) {
		return first_name + " with unknown last name";

		// если известны и имя, и фамилия
	}
	else {
		return first_name + " " + last_name;
	}
}