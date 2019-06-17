#pragma once
#include <forward_list>
#include <vector>
#include <algorithm>
#include <iterator>


template <typename Type, typename Hasher>
class HashSet {
public:
	using BucketList = std::forward_list<Type>;

	explicit HashSet(size_t num_buckets, const Hasher& hasher = {});

	void Add(const Type& value);

	bool Has(const Type& value) const;

	void Erase(const Type& value);

	const BucketList& GetBucket(const Type& value) const;

private:
	std::vector<BucketList> data_;
	Hasher hasher_;
};


struct IntHasher 
{
	size_t operator()(int value) const 
    {
		return value;
	}
};


struct TestValue 
{
	int value;

	bool operator==(TestValue other) const 
    {
		return value / 2 == other.value / 2;
	}
};


struct TestValueHasher 
{
	size_t operator()(TestValue value) const 
    {
		return value.value / 2;
	}
};


template <typename Type, typename Hasher>
HashSet<Type, Hasher>::HashSet(size_t num_buckets, const Hasher& hasher)
{
	data_ = std::vector<BucketList>(num_buckets);
	hasher_ = hasher;
}


template <typename Type, typename Hasher>
void HashSet<Type, Hasher>::Add(const Type& value)
{
	const size_t index = hasher_(value) % data_.size();

    if (data_[index].empty())
    {
		data_[index].push_front(value);
    }
    else
    {
		auto iter = data_[index].begin();

        for (; iter != data_[index].end();)
        {
            if (*iter == value)
            {
                return;
            }

            if (std::next(iter) == data_[index].end())
            {
                break;
            }

			iter = std::next(iter);
        }

		data_[index].insert_after(iter, value);
    }
}


template <typename Type, typename Hasher>
bool HashSet<Type, Hasher>::Has(const Type& value) const
{
	const size_t index = hasher_(value) % data_.size();

	return std::count(data_.at(index).begin(), data_.at(index).end(), value) != 0;
}


template <typename Type, typename Hasher>
void HashSet<Type, Hasher>::Erase(const Type& value)
{
	const size_t index = hasher_(value) % data_.size();

    if (data_[index].empty())
        return;

	auto iter = data_[index].begin();

    if (*iter == value)
    {
		data_[index].erase_after(data_[index].before_begin());
        return;
    }

    for (; iter != data_[index].end();)
    {
        if (*std::next(iter) == value)
            break;

		iter = std::next(iter);
    }

    if (iter == data_[index].end())
        return;

	data_[index].erase_after(iter);
}


template <typename Type, typename Hasher>
const typename HashSet<Type, Hasher>::BucketList& HashSet<Type, Hasher>::GetBucket(const Type& value) const
{
	return data_.at(hasher_(value) % data_.size());
}


void TestHashSet();