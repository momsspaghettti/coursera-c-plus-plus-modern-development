#include "UnitTestsFramework.h"
#include "my_deque.h"
#include <string>


void TestConstructor()
{
	Deque<int> deque;

	ASSERT_EQUAL(deque.Size(), 0u);
	ASSERT(deque.Empty());

	Deque<int> dq;
	dq.PushBack(2);
	dq.PushFront(1);
	ASSERT(!dq.Empty());

	Deque<int> dq1;
	dq1.PushBack(2);
	dq1.PushFront(1);
	ASSERT_EQUAL(dq1.Size(), 2u);
}


void TestOperators()
{
	Deque<int> deque;

	deque.PushBack(12);
	ASSERT_EQUAL(deque[0], 12);

	deque[0] = 1;
	ASSERT_EQUAL(deque[0], 1);

	deque.PushFront(12);
	ASSERT_EQUAL(deque[0], 12);
	ASSERT_EQUAL(deque[1], 1);

	Deque<int> dq_of_int;
	dq_of_int.PushFront(1);
	dq_of_int.PushFront(2);
	dq_of_int.PushFront(3);
	dq_of_int.PushBack(4);
	dq_of_int.PushBack(5);
	ASSERT_EQUAL(dq_of_int[0], 3);
	ASSERT_EQUAL(dq_of_int[1], 2);
	ASSERT_EQUAL(dq_of_int[2], 1);
	ASSERT_EQUAL(dq_of_int[3], 4);
	ASSERT_EQUAL(dq_of_int[4], 5);

	Deque<double> dq_of_db;
	dq_of_db.PushFront(1.5);
	dq_of_db.PushFront(2.5);
	dq_of_db.PushFront(3.5);
	ASSERT_EQUAL(dq_of_db[0], 3.5);
	ASSERT_EQUAL(dq_of_db[1], 2.5);
	ASSERT_EQUAL(dq_of_db[2], 1.5);

	Deque<char> dq_of_ch;
	dq_of_ch.PushBack('a');
	dq_of_ch.PushBack('b');
	ASSERT_EQUAL(dq_of_ch[0], 'a');
}


void TestAtMethod()
{
	Deque<std::string> deque;

	try
	{
		deque.At(0) = "hello";
	}
	catch (std::out_of_range& ex)
	{
		ASSERT(true);
	}

	try
	{
		auto tmp = deque.At(20);
	}
	catch (std::out_of_range& ex)
	{
		ASSERT(true);
	}

	deque.PushBack("2");
	deque.PushFront("1");

	ASSERT_EQUAL(deque.At(0), "1");
	ASSERT_EQUAL(deque.At(1), "2");

	deque.At(0) = "2";
	deque.At(1) = "1";

	ASSERT_EQUAL(deque.At(0), "2");
	ASSERT_EQUAL(deque.At(1), "1");
}


void TestFrontAndBack()
{
	Deque<std::string> deque;

	{
		try
		{
			deque.Front() = "hello";
		}
		catch (std::out_of_range& ex)
		{
			ASSERT(true);
		}

		try
		{
			auto tmp = deque.Front();
		}
		catch (std::out_of_range& ex)
		{
			ASSERT(true);
		}
	}

	{
		try
		{
			deque.Back() = "hello";
		}
		catch (std::out_of_range& ex)
		{
			ASSERT(true);
		}

		try
		{
			auto tmp = deque.Back();
		}
		catch (std::out_of_range& ex)
		{
			ASSERT(true);
		}
	}

	{
		deque.PushBack("2");
		deque.PushFront("1");

		ASSERT_EQUAL(deque.Front(), "1");
		ASSERT_EQUAL(deque.Back(), "2");

		deque.Front() = "2";
		deque.Back() = "1";

		ASSERT_EQUAL(deque.Front(), "2");
		ASSERT_EQUAL(deque.Back(), "1");
	}

	{
		Deque<int> dq;
		dq.PushFront(1);
		ASSERT_EQUAL(dq.Front(), 1);
		dq.Front() = 0;
		ASSERT_EQUAL(dq.Front(), 0);

		Deque<int> dq1;
		dq1.PushBack(1);
		ASSERT_EQUAL(dq1.Back(), 1);
		dq1.Back() = 0;
		ASSERT_EQUAL(dq1.Back(), 0);
	}
}


void TestPushes()
{
	Deque<int> deque;

    for (int i = 0; i < 5; ++i)
    {
		deque.PushBack(5 - i);
		deque.PushFront(i);

		ASSERT_EQUAL(deque.Front(), i);
		ASSERT_EQUAL(deque.Back(), 5 - i);
    }

	Deque<std::string> d;
	d.PushBack("A");
	ASSERT_EQUAL(d.Front(), "A");
	ASSERT_EQUAL(d.Back(), "A");

	Deque<int> d1;
    for (int i = 1; i < 5; ++i)
    {
		d1.PushBack(i);
		d1.PushFront(i);
    }
	ASSERT_EQUAL(d1.Front(), 4);
	ASSERT_EQUAL(d1.Back(), 4);
	for (int i = 1; i < 5; ++i)
	{
		ASSERT_EQUAL(d1[i - 1], 5 - i);
		ASSERT_EQUAL(d1[i + 3], i);
	}

	d1 = Deque<int>();
	for (int i = 1; i < 5; ++i)
	{
		d1.PushBack(i);
	}
	ASSERT_EQUAL(d1.Front(), 1);
	ASSERT_EQUAL(d1.Back(), 4);

	d1 = Deque<int>();
	for (int i = 1; i < 5; ++i)
	{
		d1.PushFront(i);
	}
	ASSERT_EQUAL(d1.Front(), 4);
	ASSERT_EQUAL(d1.Back(), 1);
}


void SomeTests()
{
    Deque<int> dq;
	dq.PushBack(1);
	ASSERT_EQUAL(dq.Back(), 1);

	Deque<int> dq2;
	dq2.PushFront(1);
	ASSERT_EQUAL(dq2.Back(), 1);

	Deque<int> dq3;
	dq3.PushFront(1);
	dq3.PushFront(2);
	ASSERT_EQUAL(dq3.Back(), 1);

	Deque<int> dq4;
	dq4.PushBack(1);
	dq4.PushFront(2);
	ASSERT_EQUAL(dq4.Back(), 1);

	Deque<int> dq5;
	dq5.PushBack(1);
	dq5.PushBack(2);
	ASSERT_EQUAL(dq5.Back(), 2);

	Deque<int> dq6;
	dq6.PushFront(2);
	dq6.PushBack(1);
	ASSERT_EQUAL(dq6.Back(), 1);
}


void TestMyDeque()
{
	TestRunner tr;
	
	RUN_TEST(tr, TestConstructor);
	RUN_TEST(tr, TestOperators);
	RUN_TEST(tr, TestAtMethod);
	RUN_TEST(tr, TestFrontAndBack);
	RUN_TEST(tr, TestPushes);
	RUN_TEST(tr, SomeTests);
}