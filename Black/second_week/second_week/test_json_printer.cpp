#include "json_printer.h"


void TestArray() {
    std::ostringstream output;

    {
        auto json = PrintJsonArray(output);
        json
                .Number(5)
                .Number(6)
                .BeginArray()
                .Number(7)
                .EndArray()
                .Number(8)
                .String("bingo!");
    }

    ASSERT_EQUAL(output.str(), R"([5,6,[7],8,"bingo!"])")
}


void TestObject() {
    std::ostringstream output;

    {
        auto json = PrintJsonObject(output);
        json
                .Key("id1").Number(1234)
                .Key("id2").Boolean(false)
                .Key("").Null()
                .Key("\"").String("\\");
    }

    ASSERT_EQUAL(output.str(), R"({"id1":1234,"id2":false,"":null,"\"":"\\"})")
}

void TestAutoClose() {
    std::ostringstream output;

    {
        auto json = PrintJsonArray(output);
        json.BeginArray().BeginObject();
    }

    ASSERT_EQUAL(output.str(), R"([[{}]])")
}


void TempObjectsTest() {
    {
        std::ostringstream output;
        PrintJsonString(output, "Hello, \"world\"");
        ASSERT_EQUAL(output.str(), R"("Hello, \"world\"")")
    }

    {
        std::ostringstream output;
        PrintJsonArray(output)
                .Null()
                .String("Hello")
                .Number(123)
                .Boolean(false)
                .EndArray();
        ASSERT_EQUAL(output.str(), R"([null,"Hello",123,false])")
    }

    {
        std::ostringstream output;
        PrintJsonArray(output)
                .Null()
                .String("Hello")
                .Number(123)
                .Boolean(false);
        ASSERT_EQUAL(output.str(), R"([null,"Hello",123,false])")
    }

    {
        std::ostringstream output;
        PrintJsonArray(output)
                .String("Hello")
                .BeginArray()
                .String("World");
        ASSERT_EQUAL(output.str(), R"(["Hello",["World"]])")
    }

    {
        std::ostringstream output;
        PrintJsonObject(output)
                .Key("foo")
                .BeginArray()
                .String("Hello")
                .EndArray()
                .Key("foo")  // повторяющиеся ключи допускаются
                .BeginObject()
                .Key("foo");
        ASSERT_EQUAL(output.str(), R"({"foo":["Hello"],"foo":{"foo":null}})")
    }

    {
        std::ostringstream output;
        {
            auto json = PrintJsonArray(output);
        }
        ASSERT_EQUAL(output.str(), R"([])")
    }

    {
        std::ostringstream output;
        {
            auto json = PrintJsonObject(output);
        }
        ASSERT_EQUAL(output.str(), R"({})")
    }

    {
        std::ostringstream output;
        {
            PrintJsonObject(output)
            .Key("x");
        }
        ASSERT_EQUAL(output.str(), R"({"x":null})")
    }

    {
        std::ostringstream output;
        PrintJsonArray(output)
                .Number(112)
                .Number(221)
                .String("hello")
                .Boolean(false)
                .Null()
                .BeginArray()
                .BeginObject()
                .Key("hello")
                .Boolean(true)
                .Key("arr")
                .BeginArray()
                .EndArray()
                .Key("obj")
                .BeginObject()
                .EndObject();
        ASSERT_EQUAL(output.str(), R"([112,221,"hello",false,null,[{"hello":true,"arr":[],"obj":{}}]])")
    }

    {
        std::ostringstream output;
        PrintJsonObject(output)
                .Key("obj")
                .BeginObject()
                .Key("obj")
                .BeginObject()
                .Key("obj")
                .BeginObject()
                .Key("obj")
                .BeginObject()
                .Key("obj")
                .BeginObject()
                .Key("obj")
                .BeginObject();
        ASSERT_EQUAL(output.str(), R"({"obj":{"obj":{"obj":{"obj":{"obj":{"obj":{}}}}}}})")
    }

    {
        std::ostringstream output;
        PrintJsonArray(output)
                .BeginArray()
                .BeginArray()
                .BeginArray()
                .BeginArray()
                .BeginArray()
                .BeginArray()
                .BeginArray()
                .BeginArray()
                .BeginObject()
                .Key("foo")
                .BeginArray()
                .String("bar");
        ASSERT_EQUAL(output.str(), R"([[[[[[[[[{"foo":["bar"]}]]]]]]]]])")
    }
}


void TestJsonPrinter() {
    TestRunner tr;
    RUN_TEST(tr, TestArray);
    RUN_TEST(tr, TestObject);
    RUN_TEST(tr, TestAutoClose);
    RUN_TEST(tr, TempObjectsTest);
}