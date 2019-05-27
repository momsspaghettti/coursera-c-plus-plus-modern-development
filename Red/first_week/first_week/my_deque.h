#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>


template<typename T>
class Deque
{
public:
	Deque() = default;

	bool Empty() const
	{
		return direct_vector_.empty() && reverse_vector_.empty();
	}

	size_t Size() const
	{
		return direct_vector_.size() + reverse_vector_.size();
	}

	const T& operator[](const size_t& index) const
	{
        if (index >= reverse_vector_.size())
        {
			return direct_vector_[index - reverse_vector_.size()];
        }
        
		return reverse_vector_[reverse_vector_.size() - index - 1];
	}

	T& operator[](const size_t& index)
	{
		if (index >= reverse_vector_.size())
		{
			return direct_vector_[index - reverse_vector_.size()];
		}

		return reverse_vector_[reverse_vector_.size() - index - 1];
	}

	const T& At(const size_t& index) const
	{
		if (index + 1 > Size() || index < 0)
		{
			throw std::out_of_range("out of range!");
		}

		return (*this)[index];
	}

	T& At(const size_t& index)
	{
		if (index + 1 > Size() || index < 0)
		{
			throw std::out_of_range("out of range!");
		}

		return (*this)[index];
	}

	const T& Front() const
	{
		return At(0);
	}

	T& Front()
	{
		return At(0);
	}

	const T& Back() const
	{
		return Empty() ? At(0) : At(Size() - 1);
	}

	T& Back()
	{
		return Empty() ? At(0) : At(Size() - 1);
	}

	void PushFront(const T& item)
	{
		reverse_vector_.push_back(item);
	}

	void PushBack(const T& item)
	{
		direct_vector_.push_back(item);
	}

private:
	std::vector<T> direct_vector_;
	std::vector<T> reverse_vector_;
};


void TestMyDeque();