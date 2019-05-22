#include "test_date.h"
#include "UnitTestsFramework.h"
#include "date.h"


using namespace std;


void TestDateOutput() {
	{
		ostringstream os;
		os << Date{ 2017, 11, 15 };
		AssertEqual(os.str(), "2017-11-15", "Date output: simple");
	}
	{
		ostringstream os;
		os << Date{ 2017, 1, 1 };
		AssertEqual(os.str(), "2017-01-01", "Date output: day and month less than 10");
	}
	{
		ostringstream os;
		os << Date{ 2, 10, 10 };
		AssertEqual(os.str(), "0002-10-10", "Date output: year less than 1000");
	}
}

void TestParseDate() {
	istringstream is("2017-11-15");
	const Date date = ParseDate(is);

	AssertEqual(date.GetYear(), 2017, "Parse date: year");
	AssertEqual(date.GetMonth(), 11, "Parse date: month");
	AssertEqual(date.GetDay(), 15, "Parse date: day");
}