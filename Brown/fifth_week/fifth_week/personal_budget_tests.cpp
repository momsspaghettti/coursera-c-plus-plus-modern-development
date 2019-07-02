#include "personal_budget.h"
#include "UnitTestsFramework.h"


void TestPersonalBudgetManager()
{
	PersonalBudgetManager budget_manager;

	budget_manager.Earn(Date().FromString("2000-01-02"),
		Date().FromString("2000-01-06"), 20);

	ASSERT_EQUAL(budget_manager.ComputeIncome(Date().FromString("2000-01-01"),
		Date().FromString("2001-01-01")), 20);

	budget_manager.PayTax(Date().FromString("2000-01-02"),
		Date().FromString("2000-01-03"));

	ASSERT_EQUAL(budget_manager.ComputeIncome(Date().FromString("2000-01-01"),
		Date().FromString("2001-01-01")), 18.96);

	budget_manager.Earn(Date().FromString("2000-01-03"),
		Date().FromString("2000-01-03"), 10);

	ASSERT_EQUAL(budget_manager.ComputeIncome(Date().FromString("2000-01-01"),
		Date().FromString("2001-01-01")), 28.96);

	budget_manager.PayTax(Date().FromString("2000-01-03"),
		Date().FromString("2000-01-03"));

	ASSERT_EQUAL(budget_manager.ComputeIncome(Date().FromString("2000-01-01"),
		Date().FromString("2001-01-01")), 27.2076);

	{
		PersonalBudgetManager manager;

		ASSERT_EQUAL(manager.ComputeIncome(Date().FromString("2018-01-01"),
			Date().FromString("2018-01-01")), 0);

		manager.Earn(Date().FromString("2018-01-01"),
			Date().FromString("2018-01-01"), 20);

		ASSERT_EQUAL(manager.ComputeIncome(Date().FromString("2018-01-01"),
			Date().FromString("2018-01-01")), 20);
		ASSERT_EQUAL(manager.ComputeIncome(Date().FromString("2017-01-01"),
			Date().FromString("2017-12-31")), 0);
		ASSERT_EQUAL(manager.ComputeIncome(Date().FromString("2018-01-02"),
			Date().FromString("2018-02-01")), 0);

		ASSERT_EQUAL(manager.ComputeIncome(Date().FromString("2017-01-01"),
			Date().FromString("2019-01-01")), 20);
	}

	{
		PersonalBudgetManager manager;

		manager.Earn(Date().FromString("2018-01-01"),
			Date().FromString("2018-01-20"), 20);

		manager.PayTax(Date().FromString("2018-01-01"),
			Date().FromString("2018-01-20"));

		ASSERT_EQUAL(manager.ComputeIncome(Date().FromString("2018-01-11"),
			Date().FromString("2018-01-20")), 8.7);

		manager.PayTax(Date().FromString("2010-01-01"),
			Date().FromString("2015-01-20"));

		ASSERT_EQUAL(manager.ComputeIncome(Date().FromString("2018-01-11"),
			Date().FromString("2018-01-20")), 8.7);

		manager.PayTax(Date().FromString("2019-01-01"),
			Date().FromString("2025-01-20"));
	}
}


void TestReadDate()
{
	std::stringstream input("2000-01-02\n2000-01-06\n2000-01-01");

	Date test_date;
	Date expected_date;

	input >> test_date;
	expected_date.year_ = 2000;
	expected_date.month_ = 1;
	expected_date.day_ = 2;
	ASSERT_EQUAL(test_date, expected_date);

	input >> test_date;
	expected_date.year_ = 2000;
	expected_date.month_ = 1;
	expected_date.day_ = 6;
	ASSERT_EQUAL(test_date, expected_date);

	input >> test_date;
	expected_date.year_ = 2000;
	expected_date.month_ = 1;
	expected_date.day_ = 1;
	ASSERT_EQUAL(test_date, expected_date);

	std::stringstream ss;
	std::string tmp =
		"2000-01-02\n"
		"2222-12-06\n"
		"2175-06-15\n";
	ss << tmp;
	
    Date dt1, dt2, dt3;
	ss >> dt1;
	ss >> dt2;
	ss >> dt3;

	expected_date.year_ = 2000;
	expected_date.month_ = 1;
	expected_date.day_ = 2;
    ASSERT_EQUAL(dt1, expected_date);

	expected_date.year_ = 2222;
	expected_date.month_ = 12;
	expected_date.day_ = 6;
	ASSERT_EQUAL(dt2, expected_date);

	expected_date.year_ = 2175;
	expected_date.month_ = 6;
	expected_date.day_ = 15;
    ASSERT_EQUAL(dt3, expected_date);
}


void TestPerformBudgetQueries()
{
	std::stringstream input;
	std::ostringstream output;

	input << 8 << "\n";
	input << "Earn 2000-01-02 2000-01-06 20\n";
	input << "ComputeIncome 2000-01-01 2001-01-01\n";
	input << "PayTax 2000-01-02 2000-01-03 13\n";
	input << "ComputeIncome 2000-01-01 2001-01-01\n";
	input << "Earn 2000-01-03 2000-01-03 10\n";
	input << "ComputeIncome 2000-01-01 2001-01-01\n";
	input << "PayTax 2000-01-03 2000-01-03 13\n";
	input << "ComputeIncome 2000-01-01 2001-01-01\n";

	PerformBudgetQueries(input, output);

	std::string expected =
		"20\n"
		"18.96\n"
		"28.96\n"
		"27.2076\n";

	ASSERT_EQUAL(output.str(), expected);
}


void TestEarn()
{
	PersonalBudgetManager budget_manager;
	
	budget_manager.Earn(Date().FromString("2000-01-01"), 
		Date(2000, 01, 01), 100);
	ASSERT_EQUAL(budget_manager.ComputeIncome(Date(2000, 01, 01), 
		Date(2000, 01, 01)), 100);

	budget_manager.Earn(Date(2000, 01, 01), 
		Date(2001, 01, 01), 367);
	ASSERT_EQUAL(budget_manager.ComputeIncome(Date(2000, 01, 02), 
		Date(2000, 01, 02)), 1);

	budget_manager.Earn(Date(2000, 03, 01), 
		Date(2000, 03, 31), 31);
	ASSERT_EQUAL(budget_manager.ComputeIncome(Date(2000, 03, 01), 
		Date(2000, 03, 31)), 62);
}


void TestPayTax() 
{
	PersonalBudgetManager hb;

    hb.Earn(Date{ 2000,01,01 }, Date{ 2000,01,01 }, 100);
	hb.PayTax(Date{ 2000,01,01 }, Date{ 2000,01,01 }, 100);
	ASSERT_EQUAL(hb.ComputeIncome(Date{ 2000,01,01 }, Date{ 2000,01,01 }), 0);

	hb.Earn(Date{ 2000,01,01 }, Date{ 2000,01,01 }, 100);
	hb.PayTax(Date{ 2000,01,01 }, Date{ 2000,01,01 });
	hb.Earn(Date{ 2000,01,01 }, Date{ 2000,01,01 }, 100);
	ASSERT_EQUAL(hb.ComputeIncome(Date{ 2000,01,01 }, Date{ 2000,01,01 }), 187);

	hb.PayTax(Date{ 2000,01,01 }, Date{ 2000,01,01 });
	ASSERT_EQUAL(hb.ComputeIncome(Date{ 2000,01,01 }, Date{ 2000,01,01 }), 162.69);

	hb.Earn(Date{ 2000,01,05 }, Date{ 2000,01,06 }, 100);
	hb.PayTax(Date{ 2000,01,05 }, Date{ 2000,01,06 });
	ASSERT_EQUAL(hb.ComputeIncome(Date{ 2000,01,05 }, Date{ 2000,01,06 }), 87);
	ASSERT_EQUAL(hb.ComputeIncome(Date{ 2000,01,05 }, Date{ 2000,01,05 }), 43.5);
}


void TestSpend()
{
	PersonalBudgetManager budget_manager;

	budget_manager.Earn(Date(2000, 01, 02), 
		Date(2000, 01, 06), 20);
	ASSERT_EQUAL(budget_manager.ComputeIncome(Date(2000, 01, 01), 
		Date(2001, 01, 01)), 20);

	budget_manager.PayTax(Date(2000, 01, 02), 
		Date(2000, 01, 03), 13);
	ASSERT_EQUAL(budget_manager.ComputeIncome(Date(2000, 01, 01), 
		Date(2001, 01, 01)), 18.96);

	budget_manager.Spend(Date(2000, 12, 30),
		Date(2001, 01, 02), 14);
	ASSERT_EQUAL(budget_manager.ComputeIncome(Date(2000, 01, 01),
		Date(2001, 01, 01)), 8.46);

	budget_manager.PayTax(Date(2000, 12, 30),
		Date(2000, 12, 30), 13);
	ASSERT_EQUAL(budget_manager.ComputeIncome(Date(2000, 01, 01),
		Date(2001, 01, 01)), 8.46);
}


void TestPersonalBudget()
{
	TestRunner tr;
	RUN_TEST(tr, TestPersonalBudgetManager);
	RUN_TEST(tr, TestReadDate);
	RUN_TEST(tr, TestPerformBudgetQueries);
	RUN_TEST(tr, TestEarn);
	RUN_TEST(tr, TestPayTax);
	RUN_TEST(tr, TestSpend);
}