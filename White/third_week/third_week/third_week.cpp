#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


using namespace std;


void abs_sort_array()
{
	int n;

	cin >> n;

	vector<int> out_vec(n);

	for (int& item : out_vec) {
		cin >> item;
	}

	sort(out_vec.begin(), out_vec.end(), [](int a, int b) {return abs(a) < abs(b); });

	for (const int& item : out_vec) {
		cout << item << " ";
	}
}


bool lower_case_comparer(string s1, string s2)
{
	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

	return s1 < s2;
}


void ignore_case_sort()
{
	int n;

	cin >> n;

	vector<string> out_vec(n);

	for (string& item : out_vec) {
		cin >> item;
	}

	sort(out_vec.begin(), out_vec.end(), lower_case_comparer);

	for (const string& item : out_vec) {
		cout << item << " ";
	}
}


class SortedStrings
{
	public:

		void AddString(const string& s) {
			strings_.push_back(s);
			sort(strings_.begin(), strings_.end());
		}
		vector<string> GetSortedStrings() {
			return strings_;
		}

	private:

		vector<string> strings_;
};


/*
 * class Person {

	public:

		void ChangeFirstName(int year, const string& first_name) {

			FullName nearest_full_name = { "", "", 0, 0};

			for (auto& item : person_story)
			{
				if (item.first <= year)
				{
					nearest_full_name = item.second;
				}
				else
				{
					if (item.second.first_name.empty() || item.second.first_name_year < year)
					{
						item.second.first_name = first_name;
						item.second.first_name_year = year;
					}
				}
			}

			person_story[year].first_name = first_name;
			person_story[year].first_name_year = year;
			person_story[year].last_name = nearest_full_name.last_name;
			person_story[year].last_name_year = nearest_full_name.last_name_year;
		}

		void ChangeLastName(int year, const string& last_name) {

			FullName nearest_full_name = { "", "" , 0, 0};

			for (auto& item : person_story)
			{
				if (item.first <= year)
				{
					nearest_full_name = item.second;
				}
				else
				{
					if (item.second.last_name.empty() || item.second.last_name_year < year)
					{
						item.second.last_name = last_name;
						item.second.last_name_year = year;
					}
				}
			}

			person_story[year].first_name = nearest_full_name.first_name;
			person_story[year].first_name_year = nearest_full_name.first_name_year;
			person_story[year].last_name = last_name;
			person_story[year].last_name_year = year;
		}

		string GetFullName(int year) {

			FullName nearest_full_name = { "", "", 0, 0 };

			for (const auto& item : person_story)
			{
				if (item.first <= year)
				{
					nearest_full_name = item.second;
				}
				else
				{
					break;
				}
			}

			if (nearest_full_name.first_name.empty() && nearest_full_name.last_name.empty())
			{
				return  "Incognito";
			}
			else if (nearest_full_name.first_name.empty())
			{
				return nearest_full_name.last_name + " with unknown first name";
			}
			else if (nearest_full_name.last_name.empty())
			{
				return nearest_full_name.first_name + " with unknown last name";
			}
			else
			{
				return nearest_full_name.first_name + " " + nearest_full_name.last_name;
			}
		}

	private:

		struct FullName
		{
			string first_name;
			string last_name;

			int first_name_year;
			int last_name_year;
		};

		map<int, FullName> person_story;
};
 */


class Person {

public:

	Person(const string& first_name, const string& last_name, const int& birthday)
	{
		birthday_first_name = first_name;
		birthday_last_name = last_name;
		this->birthday = birthday;

		first_names[birthday] = first_name;
		last_names[birthday] = last_name;
	}

	void ChangeFirstName(int year, const string& first_name) {
		if (year >= birthday)
		{
			first_names[year] = first_name;
		}
	}

	void ChangeLastName(int year, const string& last_name) {
		if (year >= birthday)
		{
			last_names[year] = last_name;
		}
	}

	string GetFullName(int year) const {
		if (year < birthday)
		{
			return "No person";
		}

		const string first_name = FindNameByYear(first_names, year);
		const string last_name = FindNameByYear(last_names, year);

		if (first_name.empty() && last_name.empty()) {
			return "Incognito";
		}
		else if (first_name.empty()) {
			return last_name + " with unknown first name";
		}
		else if (last_name.empty()) {
			return first_name + " with unknown last name";
		}
		else {
			return first_name + " " + last_name;
		}
	}

	string GetFullNameWithHistory(int year) const {
		if (year < birthday)
		{
			return "No person";
		}

		const string first_name = FindNameByYear(first_names, year);
		const string last_name = FindNameByYear(last_names, year);

		const string first_name_story = FindNamesByYear(first_names, year);
		const string last_name_story = FindNamesByYear(last_names, year);

		if (first_name.empty() && last_name.empty()) {
			return "Incognito";
		}
		else if (first_name.empty()) {
			return last_name + last_name_story + " with unknown first name";
		}
		else if (last_name.empty()) {
			return first_name + first_name_story + " with unknown last name";
		}
		else {
			return first_name + first_name_story + " " + last_name + last_name_story;
		}
	}

private:
	string birthday_first_name;
	string birthday_last_name;
	int birthday;

	map<int, string> first_names;
	map<int, string> last_names;

	string FindNameByYear(const map<int, string>& names, int year) const
	{
		string name;

		for (const auto& item : names) {
			if (item.first <= year) {
				name = item.second;
			}
			else {
				break;
			}
		}

		return name;
	}

	string FindNamesByYear(const map<int, string>& names, int year) const
	{
		string result = "";
		vector<string> names_vec;
		int nearest_year;

		for (const auto& item : names)
		{
			if (item.first <= year)
			{
				nearest_year = item.first;
				if (count(names_vec.begin(), names_vec.end(), item.second) == 0)
					names_vec.insert(names_vec.begin() + 0, item.second);
			}
			else
			{
				break;
			}
		}

		for (int i = 0; i < names_vec.size(); ++i)
		{
			if (i == 0 && names_vec[i] != names.at(nearest_year) || i != 0)
			{
				if (!result.empty())
				{
					result += ", " + names_vec[i];
				}
				else
				{
					result = names_vec[i];
				}
			}
		}

		if (!result.empty())
		{
			result = " (" + result + ")";
		}

		return result;
	}
};

class ReversibleString
{
public:
	ReversibleString(const string& str)
	{
		data = str;
	}

	ReversibleString()
	{
		data = "";
	}

	void Reverse()
	{
		reverse(data.begin(), data.end());
	}

	string ToString() const
	{
		return data;
	}

private:
	string data;
};


struct Incognizable
{
	int first = 0;
	int last = 0;
};


int main()
{
	return 0;
}