#include <sstream>
#include "UnitTestsFramework.h"
#include "sum.h"


Combiner::Combiner(int64_t f, int64_t s) : first_(f), second_(s) {}


Combiner InitCombiner(std::istream& input)
{
	int64_t f, s;

	input >> f >> s;

	return Combiner(f, s);
}


void Combiner::PerformSum()
{
    if (!sum_is_overflow())
	    result_ = first_ + second_;
}


void Combiner::PrintResult(std::ostream& output)
{
	if (result_)
	{
		output << *result_;
	}
    else
    {
		output << "Overflow!";
    }
}


bool Combiner::sum_is_overflow() const
{
    if (first_ >= 0 && second_ >= 0)
    {
		const int64_t max_ = INT64_MAX;

		const int64_t diff = max_ - first_;

		return second_ > diff;
    }
    
    if (first_ < 0 && second_ < 0)
    {
		const int64_t min_ = INT64_MIN;

		const int64_t diff = min_ - first_;

		return second_ < diff;
    }

	return false;
}


void TestReadNumbers()
{
	{
		std::istringstream input("5 -3");

		Combiner combiner = InitCombiner(input);

		ASSERT_EQUAL(combiner.first_, 5);
		ASSERT_EQUAL(combiner.second_, -3);
	}

	{
		std::istringstream input("4567890123456789012 5678901234567890123");

		Combiner combiner = InitCombiner(input);

		ASSERT_EQUAL(combiner.first_, 4567890123456789012);
		ASSERT_EQUAL(combiner.second_, 5678901234567890123);
	}
}


void TestSumLittleNumbers()
{
	{
		std::istringstream input("5 -3");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		ASSERT_EQUAL(*combiner.result_, 2);
	}

	{
		std::istringstream input("2 -3");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		ASSERT_EQUAL(*combiner.result_, -1);
	}

	{
		std::istringstream input("-3 -3");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		ASSERT_EQUAL(*combiner.result_, -6);
	}

	{
		std::istringstream input("2 2");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		ASSERT_EQUAL(*combiner.result_, 4);
	}

	{
		std::istringstream input("50 -3");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		ASSERT_EQUAL(*combiner.result_, 47);
	}
}


void TestSumIsOverflow()
{
	{
		std::istringstream input("5 -3");

		Combiner combiner = InitCombiner(input);

		ASSERT(!combiner.sum_is_overflow());
	}

	{
		std::istringstream input("4567890123456789012 5678901234567890123");

		Combiner combiner = InitCombiner(input);

		ASSERT(combiner.sum_is_overflow());
	}

	{
		std::istringstream input("-4567890123456789012 5678901234567890123");

		Combiner combiner = InitCombiner(input);

		ASSERT(!combiner.sum_is_overflow());
	}

	{
		std::istringstream input("50 -3");

		Combiner combiner = InitCombiner(input);

		ASSERT(!combiner.sum_is_overflow());
	}

	{
		int64_t f = INT64_MAX / 2;
		int64_t s = f;

		std::istringstream input(std::to_string(f) + " " + std::to_string(s));

		Combiner combiner = InitCombiner(input);

		ASSERT(!combiner.sum_is_overflow());
	}

	{
		int64_t f = INT64_MAX;
		int64_t s = f;

		std::istringstream input(std::to_string(f) + " " + std::to_string(s));

		Combiner combiner = InitCombiner(input);

		ASSERT(combiner.sum_is_overflow());
	}

	{
		int64_t f = -INT64_MAX / 2;
		int64_t s = f;

		std::istringstream input(std::to_string(f) + " " + std::to_string(s));

		Combiner combiner = InitCombiner(input);

		ASSERT(!combiner.sum_is_overflow());
	}

	{
		int64_t f = INT64_MAX / 2;
		int64_t s = -f;

		std::istringstream input(std::to_string(f) + " " + std::to_string(s));

		Combiner combiner = InitCombiner(input);

		ASSERT(!combiner.sum_is_overflow());
	}

	{
        // INT64_MIN = -INT64_MAX - 1
		int64_t f = -INT64_MAX / 2; 
		int64_t s = f - 2;

		std::istringstream input(std::to_string(f) + " " + std::to_string(s));

		Combiner combiner = InitCombiner(input);

		ASSERT(!combiner.sum_is_overflow());
	}

	{
		// INT64_MIN = -INT64_MAX - 1
		int64_t f = -INT64_MAX / 2;
		int64_t s = f - 1;

		std::istringstream input(std::to_string(f) + " " + std::to_string(s));

		Combiner combiner = InitCombiner(input);

		ASSERT(!combiner.sum_is_overflow());
	}

	{
		std::istringstream input("9000000000000000000 9000000000000000000");

		Combiner combiner = InitCombiner(input);

		ASSERT(combiner.sum_is_overflow());
	}
}


void TestPrintSum()
{
	{
		std::istringstream input("5 -3");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		std::ostringstream output;
		combiner.PrintResult(output);

		const std::string expected = "2";
		ASSERT_EQUAL(output.str(), expected);
	}

	{
		std::istringstream input("4567890123456789012 5678901234567890123");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		std::ostringstream output;
		combiner.PrintResult(output);

		const std::string expected = "Overflow!";
		ASSERT_EQUAL(output.str(), expected);
	}

	{
		std::istringstream input("-4567890123456789012 5678901234567890123");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		std::ostringstream output;
		combiner.PrintResult(output);

		const std::string expected = "1111011111111101111";
		ASSERT_EQUAL(output.str(), expected);
	}

	{
		std::istringstream input("4567890123456789012 -5678901234567890123");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		std::ostringstream output;
		combiner.PrintResult(output);

		const std::string expected = "-1111011111111101111";
		ASSERT_EQUAL(output.str(), expected);
	}

	{
		std::istringstream input("9000000000000000000 9000000000000000000");

		Combiner combiner = InitCombiner(input);
		combiner.PerformSum();

		std::ostringstream output;
		combiner.PrintResult(output);

		const std::string expected = "Overflow!";
		ASSERT_EQUAL(output.str(), expected);
	}
}


void TestSum()
{
	TestRunner tr;

	RUN_TEST(tr, TestReadNumbers);
	RUN_TEST(tr, TestSumLittleNumbers);
	RUN_TEST(tr, TestSumIsOverflow);
	RUN_TEST(tr, TestPrintSum);
}