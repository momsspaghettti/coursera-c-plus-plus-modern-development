#pragma once
#include <cstddef>  // нужно для nullptr_t
#include <utility>


// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr
{
public:
	UniquePtr()
	{
		pointer_ = nullptr;
	}

	UniquePtr(T* ptr) : pointer_(std::move(ptr)) {}

	UniquePtr(const UniquePtr&) = delete;

	UniquePtr(UniquePtr&& other) noexcept
    {
		pointer_ = std::move(other.pointer_);
		other.pointer_ = nullptr;
	}

	UniquePtr& operator = (const UniquePtr&) = delete;

	UniquePtr& operator = (nullptr_t)
	{
		delete pointer_;
		pointer_ = nullptr;
		return *this;
	}

	UniquePtr& operator = (UniquePtr&& other) noexcept
	{
		pointer_ = std::move(other.pointer_);
		other.pointer_ = nullptr;
		return *this;
	}

	~UniquePtr()
	{
		delete pointer_;
	}

	T& operator * () const
	{
		return *pointer_;
	}

	T* operator -> () const
	{
		return pointer_;
	}

	T* Release()
	{
		T* tmp = std::move(pointer_);
		pointer_ = nullptr;

		return std::move(tmp);
	}

	void Reset(T* ptr)
	{
		delete pointer_;
		pointer_ = std::move(ptr);
	}

	void Swap(UniquePtr& other)
	{
		T* tmp = std::move(pointer_);
		pointer_ = std::move(other.pointer_);
		other.pointer_ = std::move(tmp);
	}

	[[nodiscard]] T* Get() const
	{
		return pointer_;
	}

private:
	T* pointer_;
};


void TestUniquePtr();