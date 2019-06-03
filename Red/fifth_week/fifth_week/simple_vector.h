#pragma once
#include <cstdlib>
#include <algorithm>
#include <string>


template <typename T>
class SimpleVector 
{
public:
    SimpleVector();

    explicit SimpleVector(size_t size);

	SimpleVector(const SimpleVector& other);

	SimpleVector(SimpleVector&& other);

	SimpleVector<T>& operator=(const SimpleVector<T>& other);

	SimpleVector<T>& operator=(SimpleVector<T>&& other);

    T& operator[](size_t index);

	const T& operator[](size_t index) const;

    T* begin();

    T* end();

	const T* begin() const;

	const T* end() const;

    size_t Size() const;

    size_t Capacity() const;

    void PushBack(const T& value);

	void PushBack(T&& value);

	~SimpleVector();

private:
	T* data_;
	size_t size_;
	size_t capacity_;

	void recalculate_capacity();
};


template <typename T>
SimpleVector<T>::SimpleVector()
{
	data_ = nullptr;

	size_ = 0;
	capacity_ = 0;
}


template <typename T>
SimpleVector<T>::SimpleVector(size_t size)
{
	data_ = new T[size];

	size_ = size;
	capacity_ = size;
}


template <typename T>
SimpleVector<T>::SimpleVector(const SimpleVector& other) : data_(new T[other.capacity_]), 
size_(other.size_), capacity_(other.capacity_)
{
	std::copy(other.begin(), other.end(), begin());
}


template <typename T>
SimpleVector<T>::SimpleVector(SimpleVector&& other) : data_(other.data_), 
size_(other.size_), capacity_(other.capacity_)
{
	other.data_ = nullptr;
	other.size_ = other.capacity_ = 0;
}


template <typename T>
SimpleVector<T>& SimpleVector<T>::operator=(const SimpleVector<T>& other)
{
	delete[] data_;

	data_ = new T[other.size_];
	size_ = other.size_;
	capacity_ = other.capacity_;

	std::copy(other.begin(), other.end(), begin());

	return *this;
}


template <typename T>
SimpleVector<T>& SimpleVector<T>::operator=(SimpleVector<T>&& other)
{
	data_ = other.data_;
	size_ = other.size_;
	capacity_ = other.capacity_;

	other.data_ = nullptr;
	other.size_ = other.capacity_ = 0;

	return *this;
}


template <typename T>
T& SimpleVector<T>::operator[](size_t index)
{
	return data_[index];
}


template <typename T>
const T& SimpleVector<T>::operator[](size_t index) const
{
	return data_[index];
}


template <typename T>
T* SimpleVector<T>::begin()
{
	return data_;
}


template <typename T>
const T* SimpleVector<T>::begin() const
{
	return data_;
}


template <typename T>
T* SimpleVector<T>::end()
{
	return data_ + size_;
}


template <typename T>
const T* SimpleVector<T>::end() const
{
	return data_ + size_;
}


template <typename T>
size_t SimpleVector<T>::Size() const
{
	return size_;
}


template <typename T>
size_t SimpleVector<T>::Capacity() const
{
	return capacity_;
}


template <typename T>
void SimpleVector<T>::PushBack(const T& value)
{
	recalculate_capacity();
	data_[size_++] = value;
}


template <typename T>
void SimpleVector<T>::PushBack(T&& value)
{
	recalculate_capacity();
	data_[size_++] = std::move(value);
}


template <typename T>
void SimpleVector<T>::recalculate_capacity()
{
	if (data_ == nullptr)
	{
		data_ = new T[1];
		capacity_ = 1;
		return;
	}

    if (Size() == Capacity())
    {
		T* tmp = new T[Capacity() + 1];

		for (size_t i = 0; i < Size(); ++i)
		{
			tmp[i] = std::move(data_[i]);
		}

		delete[] data_;
		data_ = new T[2 * Capacity()];

		for (size_t i = 0; i < Size(); ++i)
		{
			data_[i] = std::move(tmp[i]);
		}

		delete[] tmp;
		capacity_ *= 2;
    }
}


template <typename T>
SimpleVector<T>::~SimpleVector()
{
	delete[] data_;
}


class StringNonCopyable : public std::string
{
public:
	using std::string::string;
	StringNonCopyable(std::string&& other) : std::string(move(other)) {}
	StringNonCopyable(const StringNonCopyable&) = delete;
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};


void TestSimpleVector();