#include "UnitTestsFramework.h"
#include "unique_ptr.h"


struct Item 
{
	static int counter;
	int value;
	
    Item(int v = 0) : value(v) 
    {
		++counter;
	}
	
    Item(const Item& other) : value(other.value)
    {
		++counter;
	}
	
    ~Item() 
    {
		--counter;
	}
};


int Item::counter = 0;


void TestLifetime() 
{
	Item::counter = 0;
	{
		UniquePtr<Item> ptr(new Item);
		ASSERT_EQUAL(Item::counter, 1);

		ptr.Reset(new Item);
		ASSERT_EQUAL(Item::counter, 1);
	}
	ASSERT_EQUAL(Item::counter, 0);

	{
		UniquePtr<Item> ptr(new Item);
		ASSERT_EQUAL(Item::counter, 1);

		auto rawPtr = ptr.Release();
		ASSERT_EQUAL(Item::counter, 1);

		delete rawPtr;
		ASSERT_EQUAL(Item::counter, 0);
	}
	ASSERT_EQUAL(Item::counter, 0);
}


void TestGetters() 
{
	UniquePtr<Item> ptr(new Item(42));
	ASSERT_EQUAL(ptr.Get()->value, 42);
	ASSERT_EQUAL((*ptr).value, 42);
	ASSERT_EQUAL(ptr->value, 42);
}


void TestUniquePtr() 
{
	TestRunner tr;
	RUN_TEST(tr, TestLifetime);
	RUN_TEST(tr, TestGetters);
}