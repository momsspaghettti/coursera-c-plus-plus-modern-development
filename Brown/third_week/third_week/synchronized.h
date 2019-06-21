#pragma once
#include <mutex>


template <typename T>
struct Access
{
public:
	Access(T& value, std::mutex& m) : ref_to_value(value), guard_(m) {}

	T& ref_to_value;

private:
	std::lock_guard<std::mutex> guard_;
};


template <typename T>
struct ConstAccess
{
public:
	ConstAccess(const T& value, std::mutex& m) : ref_to_value(value), guard_(m) {}

	const T& ref_to_value;

private:
	std::lock_guard<std::mutex> guard_;
};


template <typename T>
class Synchronized
{
public:
	explicit Synchronized(T initial = T()) : value_(std::move(initial)) {}

	Access<T> GetAccess()
	{
		return { value_, mutex_ };
	}

	ConstAccess<T> GetAccess() const
	{
		return { value_, mutex_ };
	}

private:
	T value_;
	mutable std::mutex mutex_;
};


void TestSynchronized();