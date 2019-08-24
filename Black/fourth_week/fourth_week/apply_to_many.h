#pragma once
#include <utility>
#include "UnitTestsFramework.h"


template <typename F, typename ...Args>
void ApplyToMany(const F& f, Args&& ...args) {
    ((f(std::forward<Args>(args))),...);
}


inline void TestSum() {
    int x;
    auto lambda = [&x](int y) { x += y; };

    x = 0;
    ApplyToMany(lambda, 1);
    ASSERT_EQUAL(x, 1)

    x = 0;
    ApplyToMany(lambda, 1, 2, 3, 4, 5);
    ASSERT_EQUAL(x, 15)
}

inline void TestConcatenate() {
    std::string s;
    auto lambda = [&s](const auto& t) {
        if (!s.empty()) {
            s += " ";
        }
        s += t;
    };

    ApplyToMany(lambda, "cyan", "magenta", "yellow", "black");
    ASSERT_EQUAL(s, "cyan magenta yellow black")
}

inline void TestIncrement() {
    auto increment = [](int& x) { ++x; };

    int a = 0;
    int b = 3;
    int c = 43;

    ApplyToMany(increment, a, b, c);
    ASSERT_EQUAL(a, 1)
    ASSERT_EQUAL(b, 4)
    ASSERT_EQUAL(c, 44)
}

inline void TestTemporary() {
    auto mover = [](std::string&& s) {
        return std::move(s);
    };
    std::string a = "a";
    std::string b = "b";
    std::string c = "c";
    ApplyToMany(mover, std::move(a), std::move(b), std::move(c));
    ASSERT(a.empty())
    ASSERT(b.empty())
    ASSERT(c.empty())
}

inline void TestApplyToMany() {
    TestRunner tr;
    RUN_TEST(tr, TestSum);
    RUN_TEST(tr, TestConcatenate);
    RUN_TEST(tr, TestIncrement);
    RUN_TEST(tr, TestTemporary);
}