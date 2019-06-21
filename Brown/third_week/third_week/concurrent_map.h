#pragma once
#include <mutex>
#include <unordered_map>
#include <vector>
#include <stdexcept>


template <typename K, typename V, typename Hash = std::hash<K>>
class ConcurrentMap
{
public:
	using MapType = std::unordered_map<K, V, Hash>;

	struct WriteAccess 
    {
	public:
        ~WriteAccess()
        {
			m.unlock();
        }

		V& ref_to_value;
		std::mutex& m;
	};

	struct ReadAccess 
    {
	public:
        ~ReadAccess()
        {
			m.unlock();
        }

		const V& ref_to_value;
		std::mutex& m;
	};

	explicit ConcurrentMap(size_t bucket_count)
	{
		mutexes_ = std::vector<std::mutex>(bucket_count);
		data_ = std::vector<MapType>(bucket_count);
	}

	WriteAccess operator[](const K& key)
	{
		const size_t index = hasher(key) % data_.size();
		mutexes_[index].lock();
		
		return { data_[index][key], mutexes_[index] };
	}

	ReadAccess At(const K& key) const
	{
		const size_t index = hasher(key) % data_.size();
		mutexes_[index].lock();

        try
        {
			const auto& value = data_.at(index).at(key);
			return { value, mutexes_[index] };
        }
        catch (std::out_of_range&)
        {
			mutexes_[index].unlock();
			throw std::out_of_range("");
        }
	}

	bool Has(const K& key) const
	{
		const size_t index = hasher(key) % data_.size();

		mutexes_[index].lock();
		const bool answer = data_.at(index).count(key) != 0;
		mutexes_[index].unlock();

		return answer;
	}

	MapType BuildOrdinaryMap() const
	{
		MapType result;

        for (size_t i = 0; i < mutexes_.size(); ++i)
        {
			mutexes_[i].lock();

            for (const auto& pair_ : data_.at(i))
            {
				result.insert(pair_);
            }

			mutexes_[i].unlock();
        }

		return result;
	}

private:
	Hash hasher;

	mutable std::vector<std::mutex> mutexes_;
	std::vector<MapType> data_;
};


void TestConcurrentMap();