#include "UnitTestsFramework.h"
#include "table.h"


void TableTests() 
{
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);

    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);

    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);

	t[0][0] = 1;
	ASSERT_EQUAL(t[0][0], 1);

	t.Resize(500, 500);
	ASSERT_EQUAL(t[0].size(), 500);
	ASSERT_EQUAL(t.Size().second, 500);

	std::vector<int> tmp;
	tmp.assign(500, 132);
	t[0] = tmp;
	ASSERT_EQUAL(t[0], tmp);

	t[499][499] = 499;
	ASSERT_EQUAL(t[499][499], 499);
}


void TestTable()
{
    TestRunner tr;
    RUN_TEST(tr, TableTests);
}
