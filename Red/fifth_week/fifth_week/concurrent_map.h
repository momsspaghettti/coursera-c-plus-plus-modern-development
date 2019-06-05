#pragma once
#include <map>
#include <vector>
#include <mutex>


template <typename K, typename V>
class ConcurrentMap 
{
public:
	static_assert(std::is_integral_v<K>, "ConcurrentMap supports only integer keys");

	struct Access 
    {
        ~Access()
        {
			mutex.unlock();
        }

		std::mutex& mutex;
		V& ref_to_value;
	};

    struct Bucket
    {
		std::mutex mutex;
		std::map<K, V> map_;
    };

	explicit ConcurrentMap(size_t bucket_count);

	Access operator[](const K& key);

	std::map<K, V> BuildOrdinaryMap();

private:
	std::vector<Bucket> buckets_;
};


template <typename K, typename V>
ConcurrentMap<K, V>::ConcurrentMap(size_t bucket_count)
{
	buckets_ = std::vector<Bucket>(bucket_count);
}


template <typename K, typename V>
typename ConcurrentMap<K, V>::Access ConcurrentMap<K, V>::operator[](const K& key)
{
	const size_t i = key % buckets_.size();
	buckets_[i].mutex.lock();

    if (buckets_[i].map_.count(key) == 0)
    {
		buckets_[i].map_.insert({ key, V() });
    }

	return { buckets_[i].mutex, buckets_[i].map_[key] };
}


template <typename K, typename V>
std::map<K, V> ConcurrentMap<K, V>::BuildOrdinaryMap()
{
	std::map<K, V> result;

    for (size_t i = 0; i < buckets_.size(); ++i)
    {
		buckets_[i].mutex.lock();

		result.insert(buckets_[i].map_.begin(), buckets_[i].map_.end());

		buckets_[i].mutex.unlock();
    }

	return result;
}


void TestConcurrentMap();