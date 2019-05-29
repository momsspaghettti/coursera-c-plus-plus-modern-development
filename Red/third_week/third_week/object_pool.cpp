#include "UnitTestsFramework.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include "object_pool.h"


template<class T>
T* ObjectPool<T>::Allocate()
{
    if (!free_pool_.empty())
    {
		auto front = free_pool_.front();
		busy_pool_.insert(front);
		free_pool_.pop();

		return front;
    }

	T* obj = new T;
	busy_pool_.insert(obj);

	return obj;
}


template <class T>
T* ObjectPool<T>::TryAllocate()
{
	if (!free_pool_.empty())
	{
		auto front = free_pool_.front();
		busy_pool_.insert(front);
		free_pool_.pop();

		return front;
	}

    return nullptr;
}


template <class T>
void ObjectPool<T>::Deallocate(T* object)
{
	const auto iter = busy_pool_.find(object);

    if (iter == busy_pool_.end())
    {
		throw std::invalid_argument("No object!");
    }

	free_pool_.push(*iter);
	busy_pool_.erase(iter);
}


template <class T>
ObjectPool<T>::~ObjectPool()
{
    while (!free_pool_.empty())
    {
		auto item = free_pool_.front();
		delete item;
		free_pool_.pop();
    }

    for (auto item : busy_pool_)
    {
		delete item;
    }

	busy_pool_.clear();
}


void ObjectPoolTests() 
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
  RUN_TEST(tr, ObjectPoolTests);
}
