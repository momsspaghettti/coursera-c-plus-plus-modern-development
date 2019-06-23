#pragma once
#include <queue>
#include <stdexcept>
#include <utility>
#include <unordered_set>


template <class T>
class ObjectPool
{
public:
	T* Allocate()
	{
	    if (!free_.empty())
	    {
			const auto iter = handled_.insert(std::move(free_.front()));
			free_.pop();

			return *(iter.first);
	    }

		T* new_object = new T;
		const auto iter = handled_.insert(std::move(new_object));

		return *(iter.first);
	}

	T* TryAllocate()
	{
		if (!free_.empty())
		{
			const auto iter = handled_.insert(std::move(free_.front()));
			free_.pop();

			return *(iter.first);
		}

        return nullptr;
	}

	void Deallocate(T* object)
	{
		const auto count = handled_.erase(object);

		if (count == 0)
			throw std::invalid_argument("!");

		free_.push(std::move(object));
	}

	~ObjectPool()
	{
        while (!free_.empty())
        {
			delete free_.front();
			free_.pop();
        }

        for (auto& ptr : handled_)
        {
			delete ptr;
        }

		handled_.clear();
	}

private:
	std::unordered_set<T*> handled_;
	std::queue<T*> free_;
};


void TestObjectPool();