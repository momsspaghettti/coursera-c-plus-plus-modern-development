#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <set>
#include <map>


using namespace std;


struct Specialization
{
	explicit Specialization(const string& new_valeu)
	{
		value = new_valeu;
	}
	string value;
};


struct Course
{
	explicit Course(const string& new_valeu)
	{
		value = new_valeu;
	}
	string value;
};


struct Week
{
	explicit Week(const string& new_valeu)
	{
		value = new_valeu;
	}
	string value;
};


struct LectureTitle {
	LectureTitle(const Specialization& new_specialization, const Course& new_course, const Week& new_week)
	{
		specialization = new_specialization.value;
		course = new_course.value;
		week = new_week.value;
	}
	string specialization;
	string course;
	string week;
};


class FunctionPart
{
public:
	FunctionPart(const char& new_operation, const double& new_value)
	{
		operation = new_operation;
		value = new_value;
	}

	double Apply(const double& source_value) const
	{
		switch (operation)
		{
		case '+':
			return source_value + value;
		case '-':
			return source_value - value;
		case '*':
			return source_value * value;
		case '/':
			return source_value / value;
		default:
			throw invalid_argument("Invalid Operation!");
		}
	}

	void Invert()
	{
		switch (operation)
		{
		case '+':
			operation = '-';
			break;
		case '-':
			operation = '+';
			break;
		case '*':
			operation = '/';
			break;
		case '/':
			operation = '*';
			break;
		default:
			throw invalid_argument("Invalid Operation!");
		}
	}
private:
	char operation;
	double value;
};


class Function
{
public:
	double Apply(double value) const
	{
		for (const auto& part : parts)
		{
			value = part.Apply(value);
		}

		return value;
	}

	void AddPart(const char& operation, const double& value)
	{
		parts.push_back({operation, value});
	}

	void Invert()
	{
		for (FunctionPart& part : parts)
		{
			part.Invert();
		}

		reverse(parts.begin(), parts.end());
	}
private:
	vector<FunctionPart> parts;
};


void ReadFile()
{
	ifstream input("input.txt");

	if(input.is_open())
	{
		string line;
		while (getline(input, line))
		{
			cout << line << endl;
		}
	}
}

void ReadWriteFile()
{
	ifstream input("input.txt");
	ofstream output("output.txt");

	if (input.is_open())
	{
		string line;
		while (getline(input, line))
		{
			output << line << endl;
		}
	}
}


void ReadNumbersFile()
{
	ifstream input("input.txt");

	if (input.is_open())
	{
		cout << fixed << setprecision(3);
		double number;

		while (input >> number)
		{
			cout << number << endl;
		}
	}
}


void ReadTable()
{
	ifstream input("input.txt");

	int n, m;

	input >> n >> m;

	string number;
	getline(input, number, '\n');

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			if (j != m - 1)
			{
				getline(input, number, ',');
				cout << setw(10) <<number << " ";
			}
			else
			{
				getline(input, number, '\n');
				cout << setw(10) << number;
			}
		}

		if (i != n - 1)
		{
			cout << endl;
		}
	}
}


void StudentList()
{
	struct Student
	{
		string name;
		string birthday;
	};

	int n;

	cin >> n;

	vector<Student> student_list;

	string first_name, last_name, day, month, year;
	for (size_t i = 0; i < n; ++i)
	{
		cin >> first_name >> last_name >> day >> month >> year;

		student_list.push_back(Student{ first_name + " " + last_name, day + "." + month + "." + year });
	}

	int m;

	cin >> m;

	for (int i = 0; i < m; ++i)
	{
		string request;
		int index;

		cin >> request >> index;
		--index;

		if (request == "name" && index < student_list.size() && index >= 0)
		{
			cout << student_list[index].name << endl;
		}
		else if (request == "date" && index < student_list.size() && index >= 0)
		{
			cout << student_list[index].birthday << endl;
		}
		else
		{
			cout << "bad request" << endl;
		}
	}
}


class Rational
{
public:
	Rational()
	{
		num = 0;
		den = 1;
	}

	Rational(const long& numerator, const long& denominator)
	{
		if (denominator == 0)
		{
			throw invalid_argument("Invalid argument");
			return;
		}

		const long div = MaxDiv(abs(numerator), abs(denominator));

		if (numerator * denominator >= 0)
		{
			if (numerator == 0)
			{
				num = numerator;
				den = 1;
			}
			else
			{
				num = abs(numerator) / div;
				den = abs(denominator) / div;
			}
		}
		else
		{
			if (numerator > 0)
			{
				num = -1 * (numerator / div);
			}
			else
			{
				num = numerator / div;
			}
			den = abs(denominator) / div;
		}
	}

	long Numerator() const
	{
		return num;
	}

	long Denominator() const
	{
		return den;
	}

	bool operator==(const Rational& other_rational) const
	{
		return (num == other_rational.Numerator()) && (den == other_rational.Denominator());
	}

	bool operator!=(const Rational& other_rational) const
	{
		return (num != other_rational.Numerator()) || (den != other_rational.Denominator());
	}

	Rational operator+(const Rational& other_rational) const
	{
		return Rational( num * other_rational.Denominator() + other_rational.Numerator() * den,
			den * other_rational.Denominator() );
	}

	Rational operator-(const Rational& other_rational) const
	{
		return Rational(num * other_rational.Denominator() - other_rational.Numerator() * den,
			den * other_rational.Denominator());
	}

	Rational operator*(const Rational& other_rational) const
	{
		return Rational(num * other_rational.Numerator(), den * other_rational.Denominator());
	}

	Rational operator/(const Rational& other_rational) const
	{
		if (other_rational.Numerator() == 0)
		{
			throw domain_error("Division by zero");
		}
		else
		{
			return Rational(num * other_rational.Denominator(), den * other_rational.Numerator());
		}
	}

	istream& operator>>(istream& stream)
	{
		long numerator = num;
		long denominator = den;

		stream >> numerator;
		stream.ignore(1);
		stream >> denominator;

		Rational tmp_obj(numerator, denominator);

		this->num = tmp_obj.Numerator();
		this->den = tmp_obj.Denominator();

		return stream;
	}

	ostream& operator<<(ostream& stream) const
	{
		stream << this->Numerator() << "/" << this->Denominator();

		return stream;
	}

	bool operator>(const Rational& other_rational) const
	{
		Rational tmp_obj(Numerator() * other_rational.Denominator() - other_rational.Numerator() * Denominator(),
			Denominator() * other_rational.Denominator());

		return tmp_obj.Numerator() > 0;
	}

	bool operator<(const Rational& other_rational) const
	{
		Rational tmp_obj(Numerator() * other_rational.Denominator() - other_rational.Numerator() * Denominator(),
			Denominator() * other_rational.Denominator());

		return tmp_obj.Numerator() < 0;
	}

private:

	long num;
	long den;

	static long MaxDiv(long a, long b)
	{
		return (b == 0) ? abs(a) : MaxDiv(b, a % b);
	}
};


istream& operator>>(istream& stream, Rational& rational)
{
	long numerator = rational.Numerator();
	long denominator = rational.Denominator();

	stream >> numerator;
	stream.ignore(1);
	stream >> denominator;

	rational = Rational(numerator, denominator);

	return stream;
}


ostream& operator<<(ostream& stream, const Rational& rational)
{
	stream << rational.Numerator() << "/" << rational.Denominator();

	return stream;
}



void EnsureEqual(const string& left, const string& right)
{
	if (left != right)
	{
		throw runtime_error(left + " != " + right);
	}
	else
	{
		return;
	}
}


void Calculator()
{
	Rational rational1;
	Rational rational2;

	char operation;

	try
	{
		cin >> rational1 >> operation >> rational2;

		switch (operation)
		{
		case '+':
			cout << rational1 + rational2;
			break;
		case '-':
			cout << rational1 - rational2;
			break;
		case '*':
			cout << rational1 * rational2;
			break;
		case '/':
			cout << rational1 / rational2;
			break;
		}
	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
	}
}


string AskTimeServer();


class TimeServer {
public:
	string GetCurrentTime()
	{
		try
		{
			string tmp_str = AskTimeServer();

			LastFetchedTime = tmp_str;

			return LastFetchedTime;
		}
		catch (system_error& ex)
		{
			return LastFetchedTime;
		}
	}

private:
	string LastFetchedTime = "00:00:00";
};


int main()
{
	int num;

	cin >> num;

	cout << num;

	cout << cin.peek();

	return 0;
}
