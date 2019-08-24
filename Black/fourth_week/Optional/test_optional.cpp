#include "optional.h"
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
        C c;
        Optional<C> o(c);
        ASSERT(C::created == 2 && C::assigned == 0 && C::deleted == 0)
    }
    ASSERT(C::deleted == 2)
}

void TestAssign() {
    Optional<C> o1, o2;

    { // Assign a Value to empty
        C::Reset();
        C c;
        o1 = c;
        ASSERT(C::created == 2 && C::assigned == 0 && C::deleted == 0)
    }
    { // Assign a non-empty to empty
        C::Reset();
        o2 = o1;
        ASSERT(C::created == 1 && C::assigned == 0 && C::deleted == 0)
    }
    { // Assign non-empty to non-empty
        C::Reset();
        o2 = o1;
        ASSERT(C::created == 0 && C::assigned == 1 && C::deleted == 0)
    }
    {
        C::Reset();
        o2 = std::move(o1);
        ASSERT(C::created == 0 && C::assigned == 1 && C::deleted == 0)
    }
}

void TestReset() {
    C::Reset();
    Optional<C> o = C();
    o.Reset();
    ASSERT(C::created == 2 && C::assigned == 0 && C::deleted == 2)
}

void TestHasValue() {
    Optional<int> o;
    ASSERT(!o.HasValue())

    o = 42;
    ASSERT(o.HasValue())

    o.Reset();
    ASSERT(!o.HasValue())
}


void TestOptional() {
    TestRunner tr;
    RUN_TEST(tr, TestInit);
    RUN_TEST(tr, TestAssign);
    RUN_TEST(tr, TestReset);
    RUN_TEST(tr, TestHasValue);
}