#pragma once
#include "UnitTestsFramework.h"


template <typename T, typename ...Args>
bool EqualsToOneOf(const T& first, const Args& ...args) {
    auto result = false;
    ((result |= first == args), ...);
    return result;
}


inline void Test() {
    auto x = "pear";
    ASSERT(!EqualsToOneOf(x))
    ASSERT(EqualsToOneOf(x, "pear"))
    ASSERT(!EqualsToOneOf(x, "apple"))
    ASSERT(EqualsToOneOf(x, "apple", "pear"))
    ASSERT(!EqualsToOneOf(x, "apple", "banana"))
    ASSERT(EqualsToOneOf(x, "apple", "banana", "pear"))
    ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach"))
    ASSERT(EqualsToOneOf(x, "apple", "banana", "pear", "orange"))
    ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach", "orange"))
    ASSERT(!EqualsToOneOf(1, 2, 3, 4, 5, 6, 7))
    ASSERT(EqualsToOneOf(1, 2, 3, 4, 5, 6, 1, 2, 10))
}


inline void TestEqualsToOneOf() {
    TestRunner tr;
    RUN_TEST(tr, Test);
}