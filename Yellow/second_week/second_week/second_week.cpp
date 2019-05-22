#include "pch.h"
#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include "UnitTestsFramework.h"


using namespace std;


int GetDistinctRealRootCount(double a, double b, double c)
{
	return 2;
}


class Person {
public:
	// Вы можете вставлять сюда различные реализации класса,
	// чтобы проверить, что ваши тесты пропускают корректный код
	// и ловят некорректный
	void ChangeFirstName(int year, const string& first_name) {
	}
	void ChangeLastName(int year, const string& last_name) {
	}
	string GetFullName(int year) {
		return "";
	}
};


class Rational {
public:
	Rational()
	{
	    
	}
	Rational(int numerator, int denominator)
	{
	    
	}

	int Numerator() const { return 0; }
	int Denominator() const { return 1; }
};


bool IsPalindrom(const string& str)
{
	return true;
}


void TestTwoRoots()
{
	AssertEqual(GetDistinctRealRootCount(1, 1, 0), 2, "a=1,b=1,c=0");
	AssertEqual(GetDistinctRealRootCount(1, 1, -10), 2, "a=1,b=1,c=-10");
	AssertEqual(GetDistinctRealRootCount(1, 0, -1), 2, "a=1,b=0,c=-1");

	AssertEqual(GetDistinctRealRootCount(-1, 0, 1), 2, "a=-1,b=0,c=1");
	AssertEqual(GetDistinctRealRootCount(-1, 1, 0), 2, "a=-1,b=-,c=0");
	AssertEqual(GetDistinctRealRootCount(-1, 1, 10), 2, "a=-1,b=1,c=10");
}


void TestOneRoot()
{
	AssertEqual(GetDistinctRealRootCount(1, 0, 0), 1, "a=1,b=0,c=0");
	AssertEqual(GetDistinctRealRootCount(1, 4, 4), 1, "a=1,b=4,c=4");

	AssertEqual(GetDistinctRealRootCount(-1, 0, 0), 1, "a=-1,b=0,c=0");
	AssertEqual(GetDistinctRealRootCount(-1, 3.5, -3.0625), 1, "a=1,b=3.5,c=-3.0625");

	AssertEqual(GetDistinctRealRootCount(0, -1.9, 0), 1, "a=0,b=-1.9,c=0");
	AssertEqual(GetDistinctRealRootCount(0, 2.1, -2), 1, "a=0,b=2.1,c=-2");
}


void TestNoRoots()
{
	AssertEqual(GetDistinctRealRootCount(1, -2.3, 1.7), 0, "a=1,b=-2.3,c=1.7");
	AssertEqual(GetDistinctRealRootCount(-10, -2.9, -3.5), 0, "a=-10,b=-2.9,c=-3.5");

	AssertEqual(GetDistinctRealRootCount(-10, 0, -3.5), 0, "a=-10,b=0,c=-3.5");
	AssertEqual(GetDistinctRealRootCount(-10, -2.9, -3.5), 0, "a=-10,b=-2.9,c=-3.5");

	AssertEqual(GetDistinctRealRootCount(0, 0, -3.5), 0, "a=0,b=0,c=-3.5");
	AssertEqual(GetDistinctRealRootCount(0, 0, 3.5), 0, "a=0,b=0,c=3.5");
}


void TestPredefinedLastNameFirst() {
	Person person;

	person.ChangeLastName(1965, "Sergeeva");
	person.ChangeFirstName(1967, "Polina");

	AssertEqual(person.GetFullName(1964), "Incognito", "Incognito");
	AssertEqual(person.GetFullName(1966), "Sergeeva with unknown first name", "Sergeeva with unknown first name");
	AssertEqual(person.GetFullName(1968), "Polina Sergeeva", "Polina Sergeeva");
}

void TestPredefined() {
	Person person;

	person.ChangeFirstName(1965, "Polina");
	person.ChangeLastName(1967, "Sergeeva");

	AssertEqual(person.GetFullName(1964), "Incognito", "Incognito");
	AssertEqual(person.GetFullName(1966), "Polina with unknown last name", "Polina with unknown last name");
	AssertEqual(person.GetFullName(1968), "Polina Sergeeva", "Polina Sergeeva");

	person.ChangeFirstName(1969, "Appolinaria");
	AssertEqual(person.GetFullName(1968), "Polina Sergeeva", "Polina Sergeeva");
	AssertEqual(person.GetFullName(1969), "Appolinaria Sergeeva", "Appolinaria Sergeeva");
	AssertEqual(person.GetFullName(1970), "Appolinaria Sergeeva", "Appolinaria Sergeeva");

	person.ChangeLastName(1968, "Volkova");
	AssertEqual(person.GetFullName(1967), "Polina Sergeeva", "Polina Sergeeva");
	AssertEqual(person.GetFullName(1968), "Polina Volkova", "Polina Volkova");
	AssertEqual(person.GetFullName(1969), "Appolinaria Volkova", "Appolinaria Volkova");

}


void TestRationalDefaultConstructor()
{
	Rational rational;

	AssertEqual(rational.Numerator(), 0, "Check numerator");
	AssertEqual(rational.Denominator(), 1, "Check denominator");
}


void TestRationalConstructor()
{
	Rational rational(13, 12);
	AssertEqual(rational.Numerator(), 13, "No reducing");
	AssertEqual(rational.Denominator(), 12, "No reducing");

	rational = Rational(12, 4);
	AssertEqual(rational.Numerator(), 3, "3/1");
	AssertEqual(rational.Denominator(), 1, "3/1");

	rational = Rational(1, 1024);
	AssertEqual(rational.Numerator(), 1, "1/1024");
	AssertEqual(rational.Denominator(), 1024, "1/1024");

	rational = Rational(17, 289);
	AssertEqual(rational.Numerator(), 1, "1/17");
	AssertEqual(rational.Denominator(), 17, "1/17");

	rational = Rational(-3, 15);
	AssertEqual(rational.Numerator(), -1, "-1/5");
	AssertEqual(rational.Denominator(), 5, "-1/5");

	rational = Rational(2, -2);
	AssertEqual(rational.Numerator(), -1, "-1/1");
	AssertEqual(rational.Denominator(), 1, "-1/1");

	rational = Rational(-2, -3);
	AssertEqual(rational.Numerator(), 2, "2/3");
	AssertEqual(rational.Denominator(), 3, "2/3");

	rational = Rational(0, 10);
	AssertEqual(rational.Numerator(), 0, "0/1");
	AssertEqual(rational.Denominator(), 1, "0/1");

	rational = Rational(0, -1);
	AssertEqual(rational.Numerator(), 0, "0/1");
	AssertEqual(rational.Denominator(), 1, "0/1");
}


void TestsIsPalindrom()
{
	Assert(IsPalindrom("madam"), "madam");
	Assert(IsPalindrom("level"), "level");
	Assert(IsPalindrom("wasitacaroracatisaw"), "wasitacaroracatisaw");

	Assert(IsPalindrom(""), "void string");

	Assert(IsPalindrom(" "), "single char");
	Assert(IsPalindrom("&"), "single char");

	Assert(IsPalindrom("   "), "only space in string");
	Assert(IsPalindrom("# ## #"), "non letters string");
	Assert(IsPalindrom("   m   "), "any string");
	Assert(IsPalindrom("abcd !!_ ! _!! dcba"), "stranger palindrom");

	Assert(IsPalindrom("  "), "two spaces");
	Assert(IsPalindrom("a b c d d c b a"), "");
	Assert(IsPalindrom("aa"), "two symbols");
}


void TestsIsNotPalindrom()
{
	Assert(!IsPalindrom("skvdlvldiewv"), "random string");
	Assert(!IsPalindrom(" m  m  "), "nearly palindrom");
	Assert(!IsPalindrom("abcd !_!! dcba"), "nearly palindrom-2");
	Assert(!IsPalindrom("abcddcba "), "ignore space");
	Assert(!IsPalindrom(" abcddcba"), "ignore space-2");
	Assert(!IsPalindrom("         -          "), "a lot of space");
	Assert(!IsPalindrom("a b c d d c b  a"), "");
	Assert(!IsPalindrom("ab"), "two symbols");
}


void TestAll()
{
	TestRunner tests;

	tests.RunTest(TestsIsPalindrom, "TestsIsPalindrom");
	tests.RunTest(TestsIsNotPalindrom, "TestsIsNotPalindrom");
}


int main() 
{
	TestAll();
	
	return 0;
}