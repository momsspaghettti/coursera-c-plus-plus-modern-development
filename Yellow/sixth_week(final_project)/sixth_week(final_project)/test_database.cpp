#include "test_database.h"
#include "database.h"
#include "UnitTestsFramework.h"
#include "date.h"


using namespace std;


void TestDatabaseAddAndPrint() {
	{
		Database db;
		db.Add({ 2017, 11, 15 }, "Wednesday");
		db.Add({ 2017, 11, 16 }, "Thursday");
		db.Add({ 2017, 11, 17 }, "Friday");

		ostringstream os;
		db.Print(os);

		const string expected = "2017-11-15 Wednesday\n"
			"2017-11-16 Thursday\n"
			"2017-11-17 Friday\n";
		AssertEqual(os.str(), expected, "Database print: 3 simple events");
	}
	{
		Database db;
		db.Add({ 2017, 3, 1 }, "1st of March");
		db.Add({ 2017, 2, 1 }, "1st of February");
		db.Add({ 2017, 1, 1 }, "1st of January");

		ostringstream os;
		db.Print(os);

		const string expected = "2017-01-01 1st of January\n"
			"2017-02-01 1st of February\n"
			"2017-03-01 1st of March\n";
		AssertEqual(os.str(), expected, "Database print: events should sorted by date");
	}
	{
		Database db;
		db.Add({ 2017, 3, 1 }, "01.03 1");
		db.Add({ 2017, 3, 5 }, "05.03 1");
		db.Add({ 2017, 3, 1 }, "01.03 2");
		db.Add({ 2017, 3, 1 }, "01.03 3");
		db.Add({ 2017, 3, 5 }, "05.03 2");

		ostringstream os;
		db.Print(os);

		const string expected = "2017-03-01 01.03 1\n"
			"2017-03-01 01.03 2\n"
			"2017-03-01 01.03 3\n"
			"2017-03-05 05.03 1\n"
			"2017-03-05 05.03 2\n";
		AssertEqual(os.str(), expected, "Database print: multiple events at single date");
	}
	{
		Database db;
		db.Add({ 2017, 3, 1 }, "01.03 1");
		db.Add({ 2017, 3, 1 }, "01.03 2");
		db.Add({ 2017, 3, 1 }, "01.03 1");
		db.Add({ 2017, 3, 1 }, "01.03 1");

		ostringstream os;
		db.Print(os);

		const string expected = "2017-03-01 01.03 1\n"
			"2017-03-01 01.03 2\n";
		AssertEqual(os.str(), expected, "Database print: identical events at the given date must be ignored");
	}
}

void TestDatabaseFind() {
	{
		const vector<pair<Date, string>> expected = {
			{{2017, 11, 17}, "Friday"},
			{{2017, 11, 18}, "Saturday"},
			{{2017, 11, 19}, "Sunday"}
		};

		Database db;
		for (const auto& e : expected) {
			db.Add(e.first, e.second);
		}

		auto alwaysTrue = [](const Date&, const string&) { return true; };
		AssertEqual(db.FindIf(alwaysTrue), expected, "Database find: All entries should be found for alwaysTrue");
	}
	{
		Database db;
		db.Add({ 2017, 11, 17 }, "Friday");
		db.Add({ 2016, 11, 17 }, "Thursday");
		db.Add({ 2015, 11, 17 }, "Tuesday");
		db.Add({ 2014, 11, 17 }, "Monday");

		auto complexCondition = [](const Date& date, const string& event) {
			return date.GetYear() == 2016 || event == "Monday";
		};
		const vector<pair<Date, string>> expected = { {{2014, 11, 17}, "Monday"}, {{2016, 11, 17}, "Thursday"} };
		AssertEqual(db.FindIf(complexCondition), expected, "Database find: complex condition");
	}
	{
		Database db;
		db.Add({ 2017, 11, 17 }, "Friday");
		db.Add({ 2016, 11, 17 }, "Thursday");
		db.Add({ 2015, 11, 17 }, "Tuesday");
		db.Add({ 2014, 11, 17 }, "Monday");

		auto acceptsNothing = [](const Date& date, const string& event) {
			return date.GetYear() == 2016 && event == "Monday";
		};
		AssertEqual(db.FindIf(acceptsNothing), vector<pair<Date, string>>(), "Database find: accepts nothing");
	}
}

void TestDatabaseRemove() {
	{
		Database db;
		db.Add({ 2017, 11, 17 }, "Friday");
		db.Add({ 2016, 11, 17 }, "Thursday");
		db.Add({ 2015, 11, 17 }, "Tuesday");
		db.Add({ 2014, 11, 17 }, "Monday");

		auto alwaysTrue = [](const Date&, const string&) { return true; };
		AssertEqual(db.RemoveIf(alwaysTrue), 4, "Database remove: alwaysTrue removes all 1");
		AssertEqual(db.FindIf(alwaysTrue), vector<pair<Date, string>>(), "Database remove: alwaysTrue removes all 2");
	}
	{
		Database db;
		db.Add({ 2017, 11, 17 }, "Friday");
		db.Add({ 2016, 11, 17 }, "Thursday");
		db.Add({ 2015, 11, 17 }, "Tuesday");
		db.Add({ 2014, 11, 17 }, "Monday");

		auto complexCondition = [](const Date& date, const string& event) {
			return date.GetYear() == 2016 || event == "Monday";
		};
		AssertEqual(db.RemoveIf(complexCondition), 2, "Database remove: complex condition 1");
		AssertEqual(db.FindIf(complexCondition), vector<pair<Date, string>>(), "Database remove: complex condition 2");
	}
	{
		const Date date = { 2017, 11, 24 };

		Database db;
		db.Add(date, "abc");
		db.Add(date, "bca");
		db.Add(date, "abd");
		db.Add(date, "cba");

		ostringstream os;
		db.Print(os);
		const string expected_one =
			"2017-11-24 abc\n"
			"2017-11-24 bca\n"
			"2017-11-24 abd\n"
			"2017-11-24 cba\n";
		AssertEqual(os.str(), expected_one, "Database print after remove 1");

		db.RemoveIf([](const Date&, const string& event) { return event[0] == 'a'; });

		os.str("");
		db.Print(os);
		const string expected_two =
			"2017-11-24 bca\n"
			"2017-11-24 cba\n";
		AssertEqual(os.str(), expected_two, "Database print after remove 2: entries should be printed in order of addition");
	}
}

void TestDatabaseLast() {
	{
		Database db;
		db.Add({ 2017, 11, 17 }, "Friday");
		db.Add({ 2017, 11, 17 }, "One more event");
		db.Add({ 2016, 11, 17 }, "Thursday");
		db.Add({ 2015, 11, 17 }, "Tuesday");
		db.Add({ 2014, 11, 17 }, "Monday");

		AssertEqual(db.Last({ 2017, 11, 17 }), pair<Date, string>{ {2017, 11, 17}, "One more event" }, "Database last: successful 1");
		AssertEqual(db.Last({ 2017, 11, 16 }), pair<Date, string>{ {2016, 11, 17}, "Thursday" }, "Database last: successful 2");
		AssertEqual(db.Last({ 2016, 11, 17 }), pair<Date, string>{ {2016, 11, 17}, "Thursday" }, "Database last: successful 3");
		AssertEqual(db.Last({ 2016, 11, 18 }), pair<Date, string>{ {2016, 11, 17}, "Thursday" }, "Database last: successful 4");
		AssertEqual(db.Last({ 2014, 11, 18 }), pair<Date, string>{ {2014, 11, 17}, "Monday" }, "Database last: successful 5");
		AssertEqual(db.Last({ 2014, 11, 17 }), pair<Date, string>{ {2014, 11, 17}, "Monday" }, "Database last: successful 6");

		bool wasException = false;
		try {
			db.Last({ 1, 1, 1 });
		}
		catch (invalid_argument&) {
			wasException = true;
		}
		catch (exception& e) {
			Assert(false, "Database last: unexpected exception " + string(e.what()));
		}
		Assert(wasException, "Database last: invalid argument wasn't thrown");
	}
	{
		Database db;
		db.Add({ 2017, 11, 17 }, "One more event");
		db.Add({ 2017, 11, 17 }, "Friday");
		AssertEqual(db.Last({ 2017, 11, 17 }), pair<Date, string>{ {2017, 11, 17}, "Friday" }, "Database last and remove 1");
		db.RemoveIf([](const Date&, const string& event) { return event == "Friday"; });
		AssertEqual(db.Last({ 2017, 11, 17 }), pair<Date, string>{ {2017, 11, 17}, "One more event" }, "Database last and remove 2");
	}
	{
		Database db;
		db.Add({ 2017, 11, 17 }, "One more event");
		db.Add({ 2017, 11, 17 }, "Friday");
		db.Add({ 2016, 11, 17 }, "Thursday");
		db.RemoveIf([](const Date& date, const string&) { return date == Date{ 2017, 11, 17 }; });
		AssertEqual(db.Last({ 2017, 11, 17 }), pair<Date, string>{ {2016, 11, 17}, "Thursday" }, "Database last and remove 2");
	}
}