#include "Common.h"
#include "UnitTestsFramework.h"
#include <sstream>


std::string Print(const Expression* e) 
{
	if (!e) 
	{
		return "Null expression provided";
	}
    std::stringstream output;
	output << e->ToString() << " = " << e->Evaluate();
	return output.str();
}


void Test() 
{
	ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
	ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

	ExpressionPtr e2 = Sum(move(e1), Value(5));
	ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

	ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}


void TestCommon() 
{
	TestRunner tr;
	RUN_TEST(tr, Test);
}