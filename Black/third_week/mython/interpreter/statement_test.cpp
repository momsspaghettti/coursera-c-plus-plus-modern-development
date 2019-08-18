#include "statement.h"
#include "UnitTestsFramework.h"
#include <sstream>
#include <string>


namespace Ast {

    using Runtime::Closure;

    template<typename T>
    void AssertObjectValueEqual(ObjectHolder obj, T expected, const std::string &msg) {
        std::ostringstream one;
        obj->Print(one);

        std::ostringstream two;
        two << expected;

        AssertEqual(one.str(), two.str(), msg);
    }

#define ASSERT_OBJECT_VALUE_EQUAL(obj, expected)                          \
{                                                                         \
  std::ostringstream __assert_equal_private_os;                           \
  __assert_equal_private_os                                               \
    << #obj << "'s value " << " != " << #expected << ", "                 \
    << FILE_NAME << ":" << __LINE__;                                      \
  AssertObjectValueEqual(obj, expected, __assert_equal_private_os.str()); \
}

    void TestNumericConst() {
        NumericConst num(Runtime::Number(57));
        Closure empty;

        ObjectHolder o = num.Execute(empty);
        ASSERT(o)
        ASSERT(empty.empty())

        std::ostringstream os;
        o->Print(os);
        ASSERT_EQUAL(os.str(), "57")
    }

    void TestStringConst() {
        StringConst value(Runtime::String("Hello!"));
        Closure empty;

        ObjectHolder o = value.Execute(empty);
        ASSERT(o)
        ASSERT(empty.empty())

        std::ostringstream os;
        o->Print(os);
        ASSERT_EQUAL(os.str(), "Hello!")
    }

    void TestVariable() {
        Runtime::Number num(42);
        Runtime::String word("Hello");

        Closure closure = {{"x", ObjectHolder::Share(num)},
                           {"w", ObjectHolder::Share(word)}};
        ASSERT(VariableValue("x").Execute(closure).Get() == &num)
        ASSERT(VariableValue("w").Execute(closure).Get() == &word)
        ASSERT_THROWS(VariableValue("unknown").Execute(closure), std::runtime_error)
    }

    void TestAssignment() {
        Assignment assign_x("x", std::make_unique<NumericConst>(Runtime::Number(57)));
        Assignment assign_y("y", std::make_unique<StringConst>(Runtime::String("Hello")));

        Closure closure = {{"y", ObjectHolder::Own(Runtime::Number(42))}};

        {
            ObjectHolder o = assign_x.Execute(closure);
            ASSERT(o)
            ASSERT_OBJECT_VALUE_EQUAL(o, 57)
        }
        ASSERT(closure.find("x") != closure.end())
        ASSERT_OBJECT_VALUE_EQUAL(closure.at("x"), 57)

        {
            ObjectHolder o = assign_y.Execute(closure);
            ASSERT(o)
            ASSERT_OBJECT_VALUE_EQUAL(o, "Hello")
        }
        ASSERT(closure.find("y") != closure.end())
        ASSERT_OBJECT_VALUE_EQUAL(closure.at("y"), "Hello")
    }

    void TestFieldAssignment() {
        Runtime::Class empty("Empty", {}, nullptr);
        Runtime::ClassInstance object{empty};

        FieldAssignment assign_x(
                VariableValue{"self"}, "x",
                std::make_unique<NumericConst>(Runtime::Number(57))
        );
        FieldAssignment assign_y(
                VariableValue{"self"}, "y",
                std::make_unique<NewInstance>(empty)
        );

        Closure closure = {{"self", ObjectHolder::Share(object)}};

        {
            ObjectHolder o = assign_x.Execute(closure);
            ASSERT(o)
            ASSERT_OBJECT_VALUE_EQUAL(o, 57)
        }
        ASSERT(object.Fields().find("x") != object.Fields().end())
        ASSERT_OBJECT_VALUE_EQUAL(object.Fields().at("x"), 57)

        assign_y.Execute(closure);
        FieldAssignment assign_yz(
                VariableValue{std::vector<std::string>{"self", "y"}}, "z",
                std::make_unique<StringConst>(
                        Runtime::String("Hello, world! Hooray! Yes-yes!!!")
                        )
                        );
        {
            ObjectHolder o = assign_yz.Execute(closure);
            ASSERT(o)
            ASSERT_OBJECT_VALUE_EQUAL(o, "Hello, world! Hooray! Yes-yes!!!")
        }

        ASSERT(object.Fields().find("y") != object.Fields().end())
        auto subobject = object.Fields().at("y").TryAs<Runtime::ClassInstance>();
        ASSERT(subobject && subobject->Fields().find("z") != subobject->Fields().end())
        ASSERT_OBJECT_VALUE_EQUAL(subobject->Fields().at("z"), "Hello, world! Hooray! Yes-yes!!!")
    }

    void TestPrintVariable() {
        std::ostringstream os;
        Print::SetOutputStream(os);

        Closure closure = {{"y", ObjectHolder::Own(Runtime::Number(42))}};

        auto print_statement = Print::Variable("y");
        print_statement->Execute(closure);

        ASSERT_EQUAL(os.str(), "42\n")
    }

    void TestPrintMultipleStatements() {
        std::ostringstream os;
        Print::SetOutputStream(os);

        Runtime::String hello("hello");
        Closure closure = {
                {"word",  ObjectHolder::Share(hello)},
                {"empty", ObjectHolder::None()}
        };

        using namespace std;

        std::vector<std::unique_ptr<Statement>> args;
        args.push_back(std::make_unique<VariableValue>("word"));
        args.push_back(std::make_unique<NumericConst>(57));
        args.push_back(std::make_unique<StringConst>("Python"s));
        args.push_back(std::make_unique<VariableValue>("empty"));

        Print(std::move(args)).Execute(closure);

        ASSERT_EQUAL(os.str(), "hello 57 Python None\n")
    }

    void TestStringify() {
        Closure empty;
        using namespace std;

        {
            auto result = Stringify(std::make_unique<NumericConst>(57)).Execute(empty);
            ASSERT_OBJECT_VALUE_EQUAL(result, "57")
            ASSERT(result.TryAs<Runtime::String>())
        }
        {
            auto result = Stringify(std::make_unique<StringConst>("Wazzup!"s)).Execute(empty);
            ASSERT_OBJECT_VALUE_EQUAL(result, "Wazzup!"s)
            ASSERT(result.TryAs<Runtime::String>())
        }
        {
            std::vector<Runtime::Method> methods;
            methods.push_back({"__str__", {}, make_unique<NumericConst>(842)});

            Runtime::Class cls("BoxedValue", std::move(methods), nullptr);

            auto result = Stringify(make_unique<NewInstance>(cls)).Execute(empty);
            ASSERT_OBJECT_VALUE_EQUAL(result, "842"s)
            ASSERT(result.TryAs<Runtime::String>())
        }
        {
            Runtime::Class cls("BoxedValue", {}, nullptr);

            Stringify str(make_unique<NewInstance>(cls));
            ASSERT_THROWS(str.Execute(empty), std::runtime_error)
        }
    }

    void TestNumbersAddition() {
        Add sum(
                std::make_unique<NumericConst>(23),
                        std::make_unique<NumericConst>(34)
                                );

        Closure empty;
        ASSERT_OBJECT_VALUE_EQUAL(sum.Execute(empty), 57)
    }

    void TestStringsAddition() {
        using namespace std;
        Add sum(
                std::make_unique<StringConst>("23"s),
                std::make_unique<StringConst>("34"s)
                        );

        Closure empty;
        ASSERT_OBJECT_VALUE_EQUAL(sum.Execute(empty), "2334")
    }

    void TestBadAddition() {
        using namespace std;
        Closure empty;

        ASSERT_THROWS(
                Add(std::make_unique<NumericConst>(42), std::make_unique<StringConst>("4"s)).Execute(empty),
                std::runtime_error
                )
        ASSERT_THROWS(
                Add(std::make_unique<StringConst>("4"s), std::make_unique<NumericConst>(42)).Execute(empty),
                std::runtime_error
                )
        ASSERT_THROWS(
                Add(std::make_unique<None>(), std::make_unique<StringConst>("4"s)).Execute(empty),
                std::runtime_error
                )
        ASSERT_THROWS(
                Add(std::make_unique<None>(), std::make_unique<None>()).Execute(empty),
                std::runtime_error
                )
    }

    void TestSuccessfullClassInstanceAdd() {
        using namespace std;
        std::vector<Runtime::Method> methods;
        methods.push_back({
            "__add__",
            {"value"},
            std::make_unique<Add>(std::make_unique<StringConst>("hello, "s),
                                  std::make_unique<VariableValue>("value"))
        });

        Runtime::Class cls("BoxedValue", std::move(methods), nullptr);

        Closure empty;
        auto result = Add(
                std::make_unique<NewInstance>(cls), std::make_unique<StringConst>("world"s)
        ).Execute(empty);
        ASSERT_OBJECT_VALUE_EQUAL(result, "hello, world")
    }

    void TestClassInstanceAddWithoutMethod() {
        using namespace std;
        Runtime::Class cls("BoxedValue", {}, nullptr);

        Closure empty;
        Add addition(
                std::make_unique<NewInstance>(cls), std::make_unique<StringConst>("world"s)
        );
        ASSERT_THROWS(addition.Execute(empty), std::runtime_error)
    }

    void TestCompound() {
        using namespace std;
        Compound cpd{
                std::make_unique<Assignment>("x", std::make_unique<StringConst>("one"s)),
                std::make_unique<Assignment>("y", std::make_unique<NumericConst>(2)),
                std::make_unique<Assignment>("z", std::make_unique<VariableValue>("x"s)),
                    };

        Closure closure;
        auto result = cpd.Execute(closure);

        ASSERT_OBJECT_VALUE_EQUAL(closure.at("x"), "one")
        ASSERT_OBJECT_VALUE_EQUAL(closure.at("y"), 2)
        ASSERT_OBJECT_VALUE_EQUAL(closure.at("z"), "one")

        ASSERT(!result)
    }

    void RunUnitTests(TestRunner &tr) {
        RUN_TEST(tr, Ast::TestNumericConst);
        RUN_TEST(tr, Ast::TestStringConst);
        RUN_TEST(tr, Ast::TestVariable);
        RUN_TEST(tr, Ast::TestAssignment);
        RUN_TEST(tr, Ast::TestFieldAssignment);
        RUN_TEST(tr, Ast::TestPrintVariable);
        RUN_TEST(tr, Ast::TestPrintMultipleStatements);
        RUN_TEST(tr, Ast::TestStringify);
        RUN_TEST(tr, Ast::TestNumbersAddition);
        RUN_TEST(tr, Ast::TestStringsAddition);
        RUN_TEST(tr, Ast::TestBadAddition);
        RUN_TEST(tr, Ast::TestSuccessfullClassInstanceAdd);
        RUN_TEST(tr, Ast::TestClassInstanceAddWithoutMethod);
        RUN_TEST(tr, Ast::TestCompound);
    }

} /* namespace Ast */
