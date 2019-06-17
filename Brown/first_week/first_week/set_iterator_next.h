#pragma once
#include <cassert>
#include <deque>
#include <iostream>


struct Node
{
	Node(int v, Node* p)
		: value(v)
		, parent(p)
	{}

	int value;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* parent;
};


class NodeBuilder 
{
public:
	Node* CreateRoot(int value)
    {
		nodes.emplace_back(value, nullptr);
		return &nodes.back();
	}

	Node* CreateLeftSon(Node* me, int value) 
    {
		assert(me->left == nullptr);
		nodes.emplace_back(value, me);
		me->left = &nodes.back();
		return me->left;
	}

	Node* CreateRightSon(Node* me, int value)
    {
		assert(me->right == nullptr);
		nodes.emplace_back(value, me);
		me->right = &nodes.back();
		return me->right;
	}

private:
    std::deque<Node> nodes;
};


inline Node* SearchDown(Node* me)
{
    if (me == nullptr)
        return nullptr;

	if (me->left == nullptr)
		return me;

	return SearchDown(me->left);
}


inline Node* SearchUp(Node* me, int value)
{
    if (me == nullptr)
        return nullptr;

	if (me->value > value)
		return me;

	return SearchUp(me->parent, value);
}



inline Node* Next(Node* me) 
{
	if (me->right)
		return SearchDown(me->right);

	return SearchUp(me->parent, me->value);
}


void TestSetIteratorNext();