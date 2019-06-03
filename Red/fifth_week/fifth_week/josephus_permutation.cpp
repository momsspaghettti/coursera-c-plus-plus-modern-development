#include "UnitTestsFramework.h"
#include "josephus_permutation.h"
#include <iterator>
#include <numeric>
#include <vector>


std::vector<int> MakeTestVector() 
{
    std::vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}


void TestIntVector() 
{
    const std::vector<int> numbers = MakeTestVector();
  
    {
        std::vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        ASSERT_EQUAL(numbers_copy, numbers);
    }
  
    {
        std::vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        ASSERT_EQUAL(numbers_copy, std::vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
    }
}


// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»


struct NoncopyableInt 
{
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};


bool operator== (const NoncopyableInt& lhs, const NoncopyableInt& rhs) 
{
    return lhs.value == rhs.value;
}


std::ostream& operator << (std::ostream& os, const NoncopyableInt& v) 
{
    return os << v.value;
}


void TestAvoidsCopying() 
{
    std::vector<NoncopyableInt> numbers;
    numbers.push_back({1});
    numbers.push_back({2});
    numbers.push_back({3});
    numbers.push_back({4});
    numbers.push_back({5});

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    std::vector<NoncopyableInt> expected;
    expected.push_back({1});
    expected.push_back({3});
    expected.push_back({5});
    expected.push_back({4});
    expected.push_back({2});

    ASSERT_EQUAL(numbers, expected);
}


void TestJosephusPermutation() 
{
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    RUN_TEST(tr, TestAvoidsCopying);
}