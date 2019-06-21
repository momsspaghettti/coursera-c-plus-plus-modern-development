#include "UnitTestsFramework.h"
#include <string>
#include "lazy_value.h"


void UseExample() 
{
	const std::string big_string = "Giant amounts of memory";

	LazyValue<std::string> lazy_string([&big_string] { return big_string; });

	ASSERT(!lazy_string.HasValue());
	ASSERT_EQUAL(lazy_string.Get(), big_string);
	ASSERT_EQUAL(lazy_string.Get(), big_string);
}


void TestInitializerIsntCalled() 
{
	bool called = false;

	{
		LazyValue<int> lazy_int([&called]
		{
			called = true;
			return 0;
		});
	}
	ASSERT(!called);
}


void TestLazyValue() 
{
	TestRunner tr;
	RUN_TEST(tr, UseExample);
	RUN_TEST(tr, TestInitializerIsntCalled);
}