#include "UnitTestsFramework.h"
#include <iostream>
#include <iomanip>


#define PRINT_VALUES(out, x, y) out << (x) << std::endl << (y) << std::endl


void PrintValuesTests()
{
	std::ostringstream output;
	PRINT_VALUES(output, 5, "red belt");
	ASSERT_EQUAL(output.str(), "5\nred belt\n");

	output = std::ostringstream("");
	PRINT_VALUES(output, 0, 0);
	ASSERT_EQUAL(output.str(), "0\n0\n");

	output = std::ostringstream("");
	if (true)
		PRINT_VALUES(output, 0, 0);
	ASSERT_EQUAL(output.str(), "0\n0\n");

	output = std::ostringstream("");
	if (true)
	{
		int tmp = 2;
		PRINT_VALUES(output, 0, 0);
	}
	ASSERT_EQUAL(output.str(), "0\n0\n");

	output = std::ostringstream("");
	if (false)
		PRINT_VALUES(output, 0, 0);
	ASSERT_EQUAL(output.str(), "");

	output = std::ostringstream("");
	if (false)
		PRINT_VALUES(output, "hello", "world");
	else
		PRINT_VALUES(output, "1", "2");
	ASSERT_EQUAL(output.str(), "1\n2\n");

	PRINT_VALUES(output, "hello", "world");
	ASSERT_EQUAL(output.str(), "1\n2\nhello\nworld\n");

	output = std::ostringstream("");
	for (int i = 0; i < 5; ++i)
		PRINT_VALUES(output, (i + 1), 5);
	PRINT_VALUES(output, "end", "test");
	ASSERT_EQUAL(output.str(), "1\n5\n2\n5\n3\n5\n4\n5\n5\n5\nend\ntest\n");

	output = std::ostringstream("");
	PRINT_VALUES(output, "", "start");
	if (true)
		PRINT_VALUES(output, "", "middle");
	PRINT_VALUES(output, "", "end");
	ASSERT_EQUAL(output.str(), "\nstart\n\nmiddle\n\nend\n");

	output = std::ostringstream("");
	PRINT_VALUES(output << std::setw(10), 77, 66);
	ASSERT_EQUAL(output.str(), "        77\n66\n");
}


void TestPrintValues()
{
    TestRunner tr;

	RUN_TEST(tr, PrintValuesTests);
}
