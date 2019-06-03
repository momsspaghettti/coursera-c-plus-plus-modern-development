#pragma once
#include <list>
#include <utility>
#include <cstdint>


template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size)
{
	std::list<typename RandomIt::value_type> pool;
	std::move(first, last, std::back_inserter(pool));

	auto list_iter = pool.begin();

	while (!pool.empty())
	{
		*(first++) = std::move(*list_iter);
		list_iter = pool.erase(std::move(list_iter));

		if (pool.empty())
		{
			break;
		}

		if (list_iter == pool.end())
		{
			list_iter = pool.begin();
		}

		for(uint32_t i = 0; i < step_size - 1; ++i)
		{
		    if (list_iter == pool.end())
		    {
				list_iter = pool.begin();
		    }
			list_iter = std::move(std::next(list_iter));
			if (list_iter == pool.end())
			{
				list_iter = pool.begin();
			}
		}
	}
}


void TestJosephusPermutation();