#include "UnitTestsFramework.h"
#include "set_iterator_next.h"


void Test1() 
{
	NodeBuilder nb;

	Node* root = nb.CreateRoot(50);
	ASSERT_EQUAL(root->value, 50);

	Node* l = nb.CreateLeftSon(root, 2);
	Node* min = nb.CreateLeftSon(l, 1);
	Node* r = nb.CreateRightSon(l, 4);
	ASSERT_EQUAL(min->value, 1);
	ASSERT_EQUAL(r->parent->value, 2);

	nb.CreateLeftSon(r, 3);
	nb.CreateRightSon(r, 5);

	r = nb.CreateRightSon(root, 100);
	l = nb.CreateLeftSon(r, 90);
	nb.CreateRightSon(r, 101);

	nb.CreateLeftSon(l, 89);
	r = nb.CreateRightSon(l, 91);

	ASSERT_EQUAL(Next(l)->value, 91);
	ASSERT_EQUAL(Next(root)->value, 89);
	ASSERT_EQUAL(Next(min)->value, 2);
	ASSERT_EQUAL(Next(r)->value, 100);

	while (min) 
	{
        std::cout << min->value << '\n';
		min = Next(min);
	}
}


void TestRootOnly() 
{
	NodeBuilder nb;
	Node* root = nb.CreateRoot(42);
	ASSERT(Next(root) == nullptr);
};


void TestSetIteratorNext() 
{
	TestRunner tr;
	RUN_TEST(tr, Test1);
	RUN_TEST(tr, TestRootOnly);
}