#include <random>
#include <stdexcept>
#include <iostream>
#include <vector>
#include "stack_vector.h"
#include "UnitTestsFramework.h"
#include "Profiler.h"


void TestConstruction() 
{
    StackVector<int, 10> v;
    ASSERT_EQUAL(v.Size(), 0u);
    ASSERT_EQUAL(v.Capacity(), 10u);

    StackVector<int, 8> u(5);
    ASSERT_EQUAL(u.Size(), 5u);
    ASSERT_EQUAL(u.Capacity(), 8u);

    try 
	{
        StackVector<int, 10> u(50);
        Assert(false, "Expect invalid_argument for too large size");
	} 
    catch (std::invalid_argument&)
    {} 
    catch (...) 
	{
        Assert(false, "Unexpected exception for too large size");
	}
}


void TestPushBack() 
{
    StackVector<int, 5> v;
  
    for (size_t i = 0; i < v.Capacity(); ++i) 
	{
        v.PushBack(i);
    }

    try 
	{
        v.PushBack(0);
        Assert(false, "Expect overflow_error for PushBack in full vector");
	} 
    catch (std::overflow_error&) 
	{}
    catch (...) 
	{
        Assert(false, "Unexpected exception for PushBack in full vector");
	}
}


void TestPopBack() 
{
    StackVector<int, 5> v;
    for (size_t i = 1; i <= v.Capacity(); ++i) 
	{
        v.PushBack(i);
    }
  
    for (int i = v.Size(); i > 0; --i) 
	{
        ASSERT_EQUAL(v.PopBack(), i);
    }

    try 
	{
        v.PopBack();
        Assert(false, "Expect underflow_error for PopBack from empty vector");
    } 
    catch (std::underflow_error&)
	{} 
    catch (...) 
	{
        Assert(false, "Unexpected exception for PopBack from empty vector");
    }
}


void TestStackVector() 
{
    {
        TestRunner tr;
        RUN_TEST(tr, TestConstruction);
        RUN_TEST(tr, TestPushBack);
        RUN_TEST(tr, TestPopBack);
    }
    
    std::cerr << "Running benchmark..." << std::endl;
    const size_t max_size = 2500;

    std::default_random_engine re;
    std::uniform_int_distribution<int> value_gen(1, max_size);

    std::vector<std::vector<int>> test_data(50000);
    for (auto& cur_vec : test_data) 
	{
        cur_vec.resize(value_gen(re));

        for (int& x : cur_vec) 
		{
            x = value_gen(re);
        }
    }

    {
        TIME_IT("vector w/o reserve");
        for (auto& cur_vec : test_data) 
		{
            std::vector<int> v;
      
            for (int x : cur_vec) 
			{
                v.push_back(x);
            }
        }
    }

    {
		TIME_IT("vector with reserve");
        for (auto& cur_vec : test_data) 
		{
            std::vector<int> v;
            v.reserve(cur_vec.size());
            for (int x : cur_vec) 
			{
                v.push_back(x);
            }
        }
    }
  
    {
		TIME_IT("StackVector");
        for (auto& cur_vec : test_data) 
		{
            StackVector<int, max_size> v;

            for (int x : cur_vec)
		    {
                v.PushBack(x);
            }
        }
    }
    
    std::cerr << "Done" << std::endl;
}
