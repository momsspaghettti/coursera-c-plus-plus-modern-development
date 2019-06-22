#include "UnitTestsFramework.h"
#include "Profiler.h"
#include <iostream>
#include <algorithm>
#include "polynomial.h"


void TestCreation()
{
	{
		Polynomial<int> default_constructed;
		ASSERT_EQUAL(default_constructed.Degree(), 0);
		ASSERT_EQUAL(default_constructed[0], 0);
	}
	
    {
		Polynomial<double> from_vector({ 1.0, 2.0, 3.0, 4.0 });
		ASSERT_EQUAL(from_vector.Degree(), 3);
		ASSERT_EQUAL(from_vector[0], 1.0);
		ASSERT_EQUAL(from_vector[1], 2.0);
		ASSERT_EQUAL(from_vector[2], 3.0);
		ASSERT_EQUAL(from_vector[3], 4.0);
	}
	
    {
		const std::vector<int> coeffs = { 4, 9, 7, 8, 12 };
		Polynomial<int> from_iterators(std::begin(coeffs), std::end(coeffs));
		ASSERT_EQUAL(from_iterators.Degree(), coeffs.size() - 1);
		ASSERT(std::equal(std::cbegin(from_iterators), std::cend(from_iterators), begin(coeffs)));
	}
}


void TestEqualComparability() 
{
	Polynomial<int> p1({ 9, 3, 2, 8 });
	Polynomial<int> p2({ 9, 3, 2, 8 });
	Polynomial<int> p3({ 1, 2, 4, 8 });

	ASSERT_EQUAL(p1, p2);
	ASSERT(p1 != p3);
}


void TestAddition()
{
	Polynomial<int> p1({ 9, 3, 2, 8 });
	Polynomial<int> p2({ 1, 2, 4 });

	p1 += p2;
	ASSERT_EQUAL(p1, Polynomial<int>({ 10, 5, 6, 8 }));

	auto p3 = p1 + p2;
	ASSERT_EQUAL(p3, Polynomial<int>({ 11, 7, 10, 8 }));

	p3 += Polynomial<int>({ -11, 1, -10, -8 });
	ASSERT_EQUAL(p3.Degree(), 1);
	const Polynomial<int> expected{ {0, 8} };
	ASSERT_EQUAL(p3, expected);
}


void TestSubtraction() 
{
	Polynomial<int> p1({ 9, 3, 2, 8 });
	Polynomial<int> p2({ 1, 2, 4 });

	p1 -= p2;
	ASSERT_EQUAL(p1, Polynomial<int>({ 8, 1, -2, 8 }));

	auto p3 = p1 - p2;
	ASSERT_EQUAL(p3, Polynomial<int>({ 7, -1, -6, 8 }));

	p3 -= Polynomial<int>({ 7, -5, -6, 8 });
	ASSERT_EQUAL(p3.Degree(), 1);
	
    const Polynomial<int> expected{ {0, 4} };
	ASSERT_EQUAL(p3, expected);
}


void TestEvaluation()
{
	const Polynomial<int> default_constructed;
	ASSERT_EQUAL(default_constructed(0), 0);
	ASSERT_EQUAL(default_constructed(1), 0);
	ASSERT_EQUAL(default_constructed(-1), 0);
	ASSERT_EQUAL(default_constructed(198632), 0);

	const Polynomial<int64_t> cubic({ 1, 1, 1, 1 });
	ASSERT_EQUAL(cubic(0), 1);
	ASSERT_EQUAL(cubic(1), 4);
	ASSERT_EQUAL(cubic(2), 15);
	ASSERT_EQUAL(cubic(21), 9724);
}


void TestPolynomialConstAccess() 
{
	const Polynomial<int> poly(std::initializer_list<int> {1, 2, 3, 4, 5});

	ASSERT_EQUAL(poly[0], 1);
	ASSERT_EQUAL(poly[1], 2);
	ASSERT_EQUAL(poly[2], 3);
	ASSERT_EQUAL(poly[3], 4);
	ASSERT_EQUAL(poly[4], 5);
	ASSERT_EQUAL(poly[5], 0);
	ASSERT_EQUAL(poly[6], 0);
	ASSERT_EQUAL(poly[608], 0);
}


void TestNonConstAccess() 
{
	Polynomial<int> poly;

	poly[0] = 1;
	poly[3] = 12;
	poly[5] = 4;
	ASSERT_EQUAL(poly.Degree(), 5);

	ASSERT_EQUAL(poly[0], 1);
	ASSERT_EQUAL(poly[1], 0);
	ASSERT_EQUAL(poly[2], 0);
	ASSERT_EQUAL(poly[3], 12);
	ASSERT_EQUAL(poly[4], 0);
	ASSERT_EQUAL(poly[5], 4);
	ASSERT_EQUAL(poly[6], 0);
	ASSERT_EQUAL(poly[608], 0);

	ASSERT_EQUAL(poly.Degree(), 5);

	poly[608] = 0;
	ASSERT_EQUAL(poly.Degree(), 5);

	{
		Polynomial<int> p;

		p[0] = 1;
		p[3] = 12;
		p[5] = 4;

		p[5] = 0;
		ASSERT_EQUAL(p.Degree(), 3);
		p[3] = 0;
		ASSERT_EQUAL(p.Degree(), 0);
	}

	{
		TIME_IT("Iteration over const");
		for (size_t i = 10; i < 50000; ++i) 
		{
			ASSERT_EQUAL(std::as_const(poly)[i], 0);
			ASSERT_EQUAL(poly.Degree(), 5);
		}
	}
	
    {
		TIME_IT("Iteration over non-const");
		for (size_t i = 10; i < 50000; ++i) 
		{
			ASSERT_EQUAL(poly[i], 0);
			ASSERT_EQUAL(poly.Degree(), 5);
		}
	}
}


void TestPolynomial()
{
	TestRunner tr;
	RUN_TEST(tr, TestCreation);
	RUN_TEST(tr, TestEqualComparability);
	RUN_TEST(tr, TestAddition);
	RUN_TEST(tr, TestSubtraction);
	RUN_TEST(tr, TestEvaluation);
	RUN_TEST(tr, TestPolynomialConstAccess);
	RUN_TEST(tr, TestNonConstAccess);
}