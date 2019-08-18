#include "object.h"
#include "statement.h"
#include "UnitTestsFramework.h"
#include <sstream>

using namespace std;

namespace Runtime {

    void TestNumber() {
        {
            Number num(127);

            ostringstream os_;
            num.Print(os_);
            ASSERT_EQUAL(os_.str(), "127")
            ASSERT_EQUAL(num.GetValue(), 127)
            ASSERT(num.IsTrue())
        }
        {
            Number num(0);

            ostringstream os_;
            num.Print(os_);
            ASSERT_EQUAL(os_.str(), "0")
            ASSERT_EQUAL(num.GetValue(), 0)
            ASSERT(!num.IsTrue())
        }
    }

    void TestString() {
        {
            String word("hello!");

            ostringstream os_;
            word.Print(os_);
            ASSERT_EQUAL(os_.str(), "hello!")
            ASSERT_EQUAL(word.GetValue(), "hello!")
            ASSERT(word.IsTrue())
        }

        {
            String word("");

            ostringstream os_;
            word.Print(os_);
            ASSERT_EQUAL(os_.str(), "")
            ASSERT_EQUAL(word.GetValue(), "")
            ASSERT(!word.IsTrue())
        }
    }

    void TestFields() {
        vector<Method> methods;

        methods.push_back({
            "__init__", {}, {
                make_unique<Ast::FieldAssignment>(
                        Ast::VariableValue{"self"},
                        "value", make_unique<Ast::NumericConst>(0)
                                )
            }
        });
        methods.push_back({
            "value", {},
            {make_unique<Ast::VariableValue>(vector<string>{"self", "value"})}
                          });
        methods.push_back({
            "add",
            {"x"},
            {
                make_unique<Ast::FieldAssignment>(
                        Ast::VariableValue{"self"},
                        "value", make_unique<Ast::Add>(
                                make_unique<Ast::VariableValue>(
                                        vector<string>{"self", "value"}),
                                        make_unique<Ast::VariableValue>("x")
                                                )
                                                )
            }
        });

        Class cls("BoxedValue", std::move(methods), nullptr);
        ClassInstance inst(cls);

        inst.Call("__init__", {});

        for (int i = 1, expected = 0; i < 10; expected += i, ++i) {
            auto fv = inst.Call("value", {});
            auto obj = fv.TryAs<Runtime::Number>();
            ASSERT(obj)
            ASSERT_EQUAL(obj->GetValue(), expected)

            inst.Call("add", {ObjectHolder::Own(Number(i))});
        }
    }

    void TestBaseClass() {
        vector<Method> methods;
        methods.push_back({
            "GetValue",{},
            make_unique<Ast::VariableValue>(vector{"self"s, "value"s})
        });
        methods.push_back({
            "SetValue", {"x"}, make_unique<Ast::FieldAssignment>(
                    Ast::VariableValue{"self"s}, "value",
                    make_unique<Ast::VariableValue>("x")
                            )
        });

        Class cls("BoxedValue", std::move(methods), nullptr);

        ASSERT_EQUAL(cls.GetName(), "BoxedValue")
        {
            auto m = cls.GetMethod("GetValue");
            ASSERT(m)
            ASSERT_EQUAL(m->name, "GetValue")
            ASSERT(m->formal_params.empty())
        }
        {
            auto m = cls.GetMethod("SetValue");
            ASSERT(m)
            ASSERT_EQUAL(m->name, "SetValue")
            ASSERT_EQUAL(m->formal_params.size(), 1u)
        }
        ASSERT(!cls.GetMethod("AsString"))
    }

    void TestInheritance() {
        vector<Method> methods;
        methods.push_back({
            "GetValue", {},
            make_unique<Ast::VariableValue>(vector{"self"s, "value"s})
        });
        methods.push_back({
            "SetValue", {"x"}, make_unique<Ast::FieldAssignment>(
                    Ast::VariableValue{"self"s}, "value", make_unique<Ast::VariableValue>("x")
                            )
        });

        Class base("BoxedValue", std::move(methods), nullptr);

        methods.clear();
        methods.push_back({
            "GetValue", {"z"}, make_unique<Ast::VariableValue>("z")
                    });
        methods.push_back({
            "AsString", {}, make_unique<Ast::StringConst>("value"s)
                    });
        Class cls("StringableValue", std::move(methods), &base);

        ASSERT_EQUAL(cls.GetName(), "StringableValue")
        {
            auto m = cls.GetMethod("GetValue");
            ASSERT(m)
            ASSERT_EQUAL(m->name, "GetValue")
            ASSERT_EQUAL(m->formal_params.size(), 1u)
        }
        {
            auto m = cls.GetMethod("SetValue");
            ASSERT(m)
            ASSERT_EQUAL(m->name, "SetValue")
            ASSERT_EQUAL(m->formal_params.size(), 1u)
        }
        {
            auto m = cls.GetMethod("AsString");
            ASSERT(m)
            ASSERT_EQUAL(m->name, "AsString")
            ASSERT(m->formal_params.empty())
        }
        ASSERT(!cls.GetMethod("AsStringValue"))
    }

    void RunObjectsTests(TestRunner &tr) {
        RUN_TEST(tr, Runtime::TestNumber);
        RUN_TEST(tr, Runtime::TestString);
        RUN_TEST(tr, Runtime::TestFields);
        RUN_TEST(tr, Runtime::TestBaseClass);
        RUN_TEST(tr, Runtime::TestInheritance);
    }

} /* namespace Runtime */
