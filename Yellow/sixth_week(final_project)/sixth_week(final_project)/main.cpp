#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "UnitTestsFramework.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include "test_database.h"
#include "test_date.h"
#include "test_node.h"


using namespace std;


string ParseEvent(istream& is) 
{
	string event_;

    if (is.peek() == ' ')
    {
		is.ignore(1);

		while (is.peek() == ' ')
		{
			is.ignore(1);
		}
    }

	getline(is, event_, '\n');

	return event_;
}


void TestAll();


int main() 
{
    TestAll();

    Database db;

    for (string line; getline(cin, line); ) 
    {
        istringstream is(line);
        string command;
        is >> command;

        if (command == "Add") 
	    {
            const auto date = ParseDate(is);
            const auto event = ParseEvent(is);
            db.Add(date, event);
	    }

        else if (command == "Print") 
		{
            db.Print(cout);
		}
        
        else if (command == "Del") 
		{
            auto condition = ParseCondition(is);

            auto predicate = [condition](const Date& date, const string& event) 
            {
                return condition->Evaluate(date, event);
            };
            
            const int count = db.RemoveIf(predicate);
            
            cout << "Removed " << count << " entries" << endl;
		} 
        
        else if (command == "Find") 
		{
            auto condition = ParseCondition(is);

            auto predicate = [condition](const Date& date, const string& event) 
            {
                return condition->Evaluate(date, event);
            };

            const auto entries = db.FindIf(predicate);

            for (const auto& entry : entries) 
			{
				cout << entry << endl;
			}
            
            cout << "Found " << entries.size() << " entries" << endl;
		} 
        
        else if (command == "Last") 
		{
            try 
			{
                cout << db.Last(ParseDate(is)) << endl;
			} 
            catch (invalid_argument&) 
			{
                cout << "No entries" << endl;
			}
		} 
        
        else if (command.empty()) 
		{
            continue;
		} 
        
        else 
		{
            throw logic_error("Unknown command: " + command);
		}
    }

    return 0;
}


void TestParseEvent() 
{
    {
        istringstream is("event");
        AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
    }

    {
        istringstream is("   sport event ");
        AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
    }

    {
        istringstream is("  first event  \n  second event");
        vector<string> events;
        events.push_back(ParseEvent(is));
        events.push_back(ParseEvent(is));
        AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
    }
}


void TestAll() 
{
    TestRunner tr;
    tr.RunTest(TestParseEvent, "TestParseEvent");
    tr.RunTest(TestParseCondition, "TestParseCondition");
	tr.RunTest(TestDateOutput, "TestDateOutput");
	tr.RunTest(TestParseDate, "TestParseDate");
	tr.RunTest(TestDatabaseAddAndPrint, "TestDatabaseAddAndPrint");
	tr.RunTest(TestDatabaseFind, "TestDatabaseFind");
	tr.RunTest(TestDatabaseRemove, "TestDatabaseRemove");
	tr.RunTest(TestDatabaseLast, "TestDatabaseLast");
	tr.RunTest(TestDateComparisonNode, "TestDateComparisonNode");
	tr.RunTest(TestEventComparisonNode, "TestEventComparisonNode");
	tr.RunTest(TestLogicalOperationNode, "TestLogicalOperationNode");
}