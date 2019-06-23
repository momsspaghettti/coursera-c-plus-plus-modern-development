#include "UnitTestsFramework.h"
#include <string>
#include "object_pool.h"


void ObjectPoolTest() 
{
	ObjectPool<std::string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}


void TestObjectPool() 
{
	TestRunner tr;
	RUN_TEST(tr, ObjectPoolTest);
}