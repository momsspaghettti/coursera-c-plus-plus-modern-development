#pragma once
#include <cstdlib>


template <typename T>
class SimpleVector 
{
public:
    SimpleVector();

    explicit SimpleVector(size_t size);

    T& operator[](size_t index);

	const T& operator[](size_t index) const;

    T* begin();

    T* end();

	const T* begin() const;

	const T* end() const;

    size_t Size() const;

    size_t Capacity() const;

    void PushBack(const T& value);

	~SimpleVector();

private:
	T* data_;
	size_t size_;
	size_t capacity_;
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
    if (data_ == nullptr)
    {
		data_ = new T[1];
		size_ = 1;
		capacity_ = 1;
		(*this)[0] = value;
        return;
    }

    if (Size() == Capacity())
    {
		T* tmp = new T[Capacity() + 1];

		for (size_t i = 0; i < Size(); ++i)
		{
			tmp[i] = data_[i];
		}
		tmp[Size()] = value;

		delete[] data_;
		data_ = new T[2 * Capacity()];

		for (size_t i = 0; i < Size(); ++i)
		{
			data_[i] = tmp[i];
		}
		data_[Size()] = value;
		delete[] tmp;

		capacity_ *= 2;
		++size_;
    }
    else
    {
		data_[Size()] = value;
		++size_;
    }
}


template <typename T>
SimpleVector<T>::~SimpleVector()
{
	delete[] data_;
}


void TestSimpleVector();