#include "UnitTestsFramework.h"
#include "Profiler.h"
#include <algorithm>
#include <numeric>
#include <string>
#include <random>
#include "concurrent_map.h"
#include <future>


void RunConcurrentUpdates(ConcurrentMap<int, int>& cm, size_t thread_count, int key_count) 
{
	auto kernel = [&cm, key_count](int seed) 
    {
        std::vector<int> updates(key_count);
		iota(begin(updates), end(updates), -key_count / 2);
		shuffle(begin(updates), end(updates), std::default_random_engine(seed));

		for (int i = 0; i < 2; ++i) 
		{
			for (auto key : updates) 
			{
				cm[key].ref_to_value++;
			}
		}
	};

	std::vector<std::future<void>> futures;
	for (size_t i = 0; i < thread_count; ++i) 
	{
		futures.push_back(std::async(kernel, i));
	}
}


void TestConcurrentUpdate() 
{
	const size_t thread_count = 8;
	const size_t key_count = 50000;

	ConcurrentMap<int, int> cm(thread_count);
	RunConcurrentUpdates(cm, thread_count, key_count);

	const auto result = cm.BuildOrdinaryMap();
	ASSERT_EQUAL(result.size(), key_count);
	for (auto& [k, v] : result) 
	{
		AssertEqual(v, 16, "Key = " + std::to_string(k));
	}
}


void TestReadAndWrite() 
{
	ConcurrentMap<size_t, std::string> cm(8);

	auto updater = [&cm] 
    {
		for (size_t i = 0; i < 50000; ++i) 
		{
			cm[i].ref_to_value += 'a';
		}
	};

	auto reader = [&cm] 
    {
		std::vector<std::string> result(50000);
		for (size_t i = 0; i < result.size(); ++i) 
		{
			result[i] = cm[i].ref_to_value;
		}
		return result;
	};

	auto u1 = std::async(updater);
	auto r1 = std::async(reader);
	auto u2 = std::async(updater);
	auto r2 = std::async(reader);

	u1.get();
	u2.get();

	for (auto f : { &r1, &r2 }) 
	{
		auto result = f->get();
		ASSERT(std::all_of(result.begin(), result.end(), [](const std::string& s) 
		{
			return s.empty() || s == "a" || s == "aa";
		}));
	}
}


void TestSpeedup() 
{
	{
		ConcurrentMap<int, int> single_lock(1);

		TIME_IT("Single lock");
		RunConcurrentUpdates(single_lock, 8, 5000);
	}

	{
		ConcurrentMap<int, int> many_locks(100);

		TIME_IT("100 locks");
		RunConcurrentUpdates(many_locks, 8, 5000);
	}
}


void TestConcurrentMap()
{
	TestRunner tr;
	RUN_TEST(tr, TestConcurrentUpdate);
	RUN_TEST(tr, TestReadAndWrite);
	RUN_TEST(tr, TestSpeedup);
}