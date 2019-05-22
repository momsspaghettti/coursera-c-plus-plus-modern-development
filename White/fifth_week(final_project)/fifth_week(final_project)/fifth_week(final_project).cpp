#include "pch.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <iomanip>
#include <set>
#include <map>
#include <sstream>


using namespace std;


class Date 
{
public:
	Date()
	{
		year = 0;
		month = 0;
		day = 0;
	}

	Date(int new_year, int new_month, int new_day)
	{
		year = new_year;

		if (new_month >= 1 && new_month <= 12)
		{
			month = new_month;
		}
		else
		{
			throw runtime_error("Month value is invalid: " + to_string(new_month));
			return;
		}

		if (new_day >= 1 && new_day <= 31)
		{
			day = new_day;
		}
		else
		{
			throw runtime_error("Day value is invalid: " + to_string(new_day));
			return;
		}
	}

	int GetYear() const
	{
		return year;
	}

	int GetMonth() const
	{
		return month;
	}

	int GetDay() const
	{
		return day;
	}

private:
	int year;
	int month;
	int day;
};


bool operator<(const Date& lhs, const Date& rhs)
{
	long total1 = lhs.GetYear() * 365 + lhs.GetMonth() * 31 + lhs.GetDay();
	long total2 = rhs.GetYear() * 365 + rhs.GetMonth() * 31 + rhs.GetDay();

	return total1 < total2;
}


istringstream& operator>>(istringstream& stream, Date& date)
{
	string line;
	istringstream date_stream;

	stream >> line;

	date_stream.str(line);

	int year, month, day;

	date_stream >> year;
	if (date_stream.peek() != '-')
	{
		throw runtime_error("Wrong date format: " + line);
	}
	else
	{
		date_stream.ignore(1);
	}
	date_stream >> month;
	if (date_stream.peek() != '-')
	{
		throw runtime_error("Wrong date format: " + line);
	}
	else
	{
		date_stream.ignore(1);
	}

	set<char> ch_vec = { '-', '+', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	if (ch_vec.count(char(date_stream.peek())) == 0)
	{
		throw runtime_error("Wrong date format: " + line);
	}
	else
	{
		date_stream >> day;
	}

	if (date_stream.peek() != EOF)
	{
		throw runtime_error("Wrong date format: " + line);
	}
	else
	{
		date = Date(year, month, day);
		return stream;
	}
}


ostream& operator<<(ostream& stream, const Date& date)
{
	stream << setw(4) << setfill('0') << date.GetYear() << "-" << setw(2) << setfill('0') <<
		date.GetMonth() << "-" << setw(2) << setfill('0') << date.GetDay();

	return stream;
}


ostream& operator<<(ostream& stream, const set<string>& str_set)
{
	const int count = str_set.size();
	int i = 1;

	for (const string& str : str_set)
	{
		stream << str;

		if (i < count)
		{
			stream << endl;
		}

		++i;
	}

	return stream;
}


class Database
{
public:
	void AddEvent(const Date& date, const string& event)
	{
		data[date].insert(event);
	}

	bool DeleteEvent(const Date& date, const string& event)
	{
		if (data.count(date) > 0)
		{
			if (data.at(date).count(event) > 0)
			{
				data.at(date).erase(event);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	int  DeleteDate(const Date& date)
	{
		if (data.count(date) > 0)
		{
			int count = data.at(date).size();

			data.erase(date);
			return count;
		}
		else
		{
			return 0;
		}
	}

	set<string> Find(const Date& date) const
	{
		if (data.count(date) > 0)
		{
			return data.at(date);
		}
		else
		{
			return {};
		}
	}

	void Print() const
	{
		for (const auto& item : data)
		{
			int m = 1;
			for (const string& event : item.second)
			{
				cout << item.first << " " << event << endl;
			}
		}
	}

private:
	map<Date, set<string>> data;
};

int main()
{
	Database db;

	string line;

	while (getline(cin, line))
	{
		string command;
		istringstream cin_;
		cin_.str(line);

		cin_ >> command;

		if (command == "Add")
		{
			try
			{
				Date date;
				string event;

				cin_ >> date >> event;

				db.AddEvent(date, event);
			}
			catch (exception& ex)
			{
				cout << ex.what() << endl;
			}
		}
		else if (command == "Del")
		{
			try
			{
				Date date;

				cin_ >> date;

				if (cin_.peek() == ' ')
				{
					string event;

					cin_ >> event;

					if (db.DeleteEvent(date, event))
					{
						cout << "Deleted successfully" << endl;
					}
					else
					{
						cout << "Event not found" << endl;
					}
				}
				else
				{
					cout << "Deleted " + to_string(db.DeleteDate(date)) + " events" << endl;
				}
			}
			catch (exception& ex)
			{
				cout << ex.what() << endl;
			}
		}
		else if (command == "Find")
		{
			try
			{
				Date date;

				cin_ >> date;

				cout << db.Find(date) << endl;;
			}
			catch (exception& ex)
			{
				cout << ex.what() << endl;
			}
		}
		else if (command == "Print")
		{
			db.Print();
		}
		else if (!command.empty())
		{
			cout << "Unknown command: " << command << endl;
		}
	}

	return 0;
}