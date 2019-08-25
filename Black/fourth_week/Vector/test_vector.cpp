#include <string>
#include "vector.h"
#include "UnitTestsFramework.h"


class C {
public:
    inline static int created = 0;
    inline static int assigned = 0;
    inline static int deleted = 0;

    static void Reset() {
        created = assigned = deleted = 0;
    }

    C() {
        ++created;
    }

    C(const C &other) {
        ++created;
    }

    C &operator=(const C &other) {
        ++assigned;
        return *this;
    }

    ~C() {
        ++deleted;
    }
};

void TestInit() {
    {
        C::Reset();
        Vector<C> v(3);
        ASSERT(C::created == 3 && C::assigned == 0 && C::deleted == 0)
    }
    ASSERT(C::deleted == 3)
}


void TestAssign() {
    {
        C::Reset();
        Vector<C> v1(2), v2(3);
        ASSERT(C::created == 5 && C::assigned == 0 && C::deleted == 0)
        v1 = v2;
        ASSERT(C::created == 8 && C::assigned == 0 && C::deleted == 2)
        ASSERT(v1.Size() == 3 && v2.Size() == 3)
    }
    ASSERT(C::deleted == 8)

    {
        C::Reset();
        Vector<C> v1(3), v2(2);
        ASSERT(C::created == 5 && C::assigned == 0 && C::deleted == 0)
        v1 = v2;
        ASSERT(C::created == 5 && C::assigned == 2 && C::deleted == 1)
        ASSERT(v1.Size() == 2 && v2.Size() == 2)
    }
    ASSERT(C::deleted == 5)
}

void TestPushBack() {
    {
        C::Reset();
        Vector<C> v;
        C c;
        v.PushBack(c);
        ASSERT(C::created == 2 && C::assigned == 0 && C::deleted == 0)

        v.PushBack(c);  // reallocation
        ASSERT(C::created == 4 && C::assigned == 0 && C::deleted == 1)
    }
    ASSERT(C::deleted == 4)

    {
        Vector<std::string> v;
        std::string s = "hello";
        v.PushBack(std::move(s));
        ASSERT_EQUAL(v.Size(), 1)
        ASSERT_EQUAL(v.Capacity(), 1)
        ASSERT(s.empty())
    }
}


void TestEmplaceBack() {
    struct Tmp {
        Tmp(int a, const std::string &b, double c, bool d) {
            this->a = a;
            this->b = b;
            this->c = c;
            this->d = d;
        }

        int a;
        std::string b;
        double c;
        bool d;
    };

    Vector<Tmp> v;
    v.EmplaceBack(1, "2", 3.0, true);
    ASSERT_EQUAL(v.Capacity(), 1)
    ASSERT_EQUAL(v.Size(), 1)
    auto tmp = v[0];
    v.PopBack();
    ASSERT_EQUAL(v.Capacity(), 1)
    ASSERT_EQUAL(v.Size(), 0)
    ASSERT_EQUAL(tmp.a, 1)
    ASSERT_EQUAL(tmp.b, "2")
    ASSERT_EQUAL(tmp.c, 3.0)
    ASSERT(tmp.c)
}


void TestResize() {
    {
        std::vector<int> target(100);
        Vector<int> source(100);
        target.resize(10);
        source.Resize(10);
        ASSERT_EQUAL(source.Capacity(), target.capacity())
        ASSERT_EQUAL(source.Size(), target.size())
    }
    {
        std::vector<int> target(10);
        Vector<int> source(10);
        target.resize(100);
        source.Resize(100);
        ASSERT_EQUAL(source.Capacity(), target.capacity())
        ASSERT_EQUAL(source.Size(), target.size())
    }
    {
        std::vector<int> target(100);
        Vector<int> source(100);
        target.resize(100);
        source.Resize(100);
        ASSERT_EQUAL(source.Capacity(), target.capacity())
        ASSERT_EQUAL(source.Size(), target.size())
    }
}


class ClassWithStrangeConstructor {
public:
    int x, y;

    ClassWithStrangeConstructor(int &r, const int &cr) : x(r), y(cr) {
    }
};

void TestInsert() {
    {
        Vector<int> v;
        v.PushBack(1);
        v.PushBack(2);
        auto it = v.Insert(v.cbegin(), 0);
        ASSERT(v.Size() == 3 && v[0] == 0 && v[1] == 1 && v[2] == 2 && it == v.begin())

        it = v.Insert(v.cend(), 3);
        ASSERT(v.Size() == 4 && v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 3 && it + 1 == v.end())
    }
    {
        Vector<int> v;
        v.PushBack(0);
        v.PushBack(2);
        auto it = v.Insert(v.cbegin() + 1, 1);
        ASSERT(v.Size() == 3 && v[0] == 0 && v[1] == 1 && v[2] == 2 && it == v.begin() + 1)

        it = v.Insert(v.cbegin() + 3, 3);
        ASSERT(v.Size() == 4 && v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 3 && it + 1 == v.end())
    }
    {
        Vector<int> v;
        for (size_t i = 0; i <= 100; ++i) {
            if (i % 2 == 0) {
                auto it = v.Insert(v.cbegin(), i);
                ASSERT(v.Size() == i + 1 && *v.cbegin() == i && v[0] == i && it == v.begin())
            } else {
                auto it = v.Insert(v.cend(), i);
                ASSERT(v.Size() == i + 1 && *v.cbegin() == i - 1 && it + 1 == v.end() && *(v.end() - 1) == i)
            }
        }
    }
}

void TestEmplace() {
    Vector<ClassWithStrangeConstructor> v;
    int x = 1;
    const int y = 2;
    int z = 3;
    ClassWithStrangeConstructor c(z, z);
    v.PushBack(c);
    auto it = v.Emplace(v.cbegin(), x, y);
    ASSERT(v.Size() == 2 && v[0].x == x && v[0].y == y && v[1].x == z && v[1].y == z && it == v.begin())
}

void TestErase() {
    Vector<int> v;
    v.PushBack(1);
    v.PushBack(2);
    v.PushBack(3);
    v.PushBack(4);
    v.PushBack(5);
    v.PushBack(6);
    for (size_t i = 0; i < 6; ++i) {
        auto it = v.Erase(v.cbegin());
        if (i != 5) {
            ASSERT(v.Size() == 6 - i - 1 && v[0] == i + 2 && it == v.begin() && it + v.Size() == v.end())
        } else {
            ASSERT(v.Size() == 0 && it == v.begin() && it == v.end() && v.begin() == v.end())
        }
    }
}


void TestVector() {
    TestRunner tr;

    RUN_TEST(tr, TestInit);
    RUN_TEST(tr, TestAssign);
    RUN_TEST(tr, TestPushBack);
    RUN_TEST(tr, TestEmplaceBack);
    RUN_TEST(tr, TestResize);
    RUN_TEST(tr, TestInsert);
    RUN_TEST(tr, TestEmplace);
    RUN_TEST(tr, TestErase);
}