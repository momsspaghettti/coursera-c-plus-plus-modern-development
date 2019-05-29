#pragma once
#include <queue>
#include <set>


template <class T>
class ObjectPool
{
public:
	T* Allocate();
	T* TryAllocate();

	void Deallocate(T* object);

	~ObjectPool();

private:
	std::queue<T*> free_pool_;
	std::set<T*> busy_pool_;
};


void TestObjectPool();