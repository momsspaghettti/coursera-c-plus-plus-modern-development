#pragma once
#include <functional>
#include <utility>
#include <optional>


template <typename T>
class LazyValue
{
public:
	explicit LazyValue(std::function<T()> init);

	bool HasValue() const;
	const T& Get() const;

private:
	mutable std::optional<T> value_ = std::nullopt;
	std::function<T()> init_;
};


template <typename T>
LazyValue<T>::LazyValue(std::function<T()> init) : init_(std::move(init)) {}


template <typename T>
bool LazyValue<T>::HasValue() const
{
	return value_.has_value();
}


template <typename T>
const T& LazyValue<T>::Get() const
{
	if (!value_)
	{
		value_ = init_();
	}

	return *value_;
}


void TestLazyValue();