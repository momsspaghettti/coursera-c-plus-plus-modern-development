#pragma once
#include "save_load.h"
#include "UnitTestsFramework.h"
#include <map>
#include <sstream>
#include <string>


void TestSaveAndLoad() {
    std::map<uint32_t, std::string> m = {
            {1, "hello"},
            {2, "bye"},
    };
    std::stringstream ss;
    Serialize(m, ss);
    auto s = ss.str();

    ASSERT_EQUAL(s.size(), 40)

    // Размер map (8 байт, так как это size_t)
    ASSERT_EQUAL(s[0],  0x02)
    ASSERT_EQUAL(s[1],  0x00)
    ASSERT_EQUAL(s[2],  0x00)
    ASSERT_EQUAL(s[3],  0x00)
    ASSERT_EQUAL(s[4],  0x00)
    ASSERT_EQUAL(s[5],  0x00)
    ASSERT_EQUAL(s[6],  0x00)
    ASSERT_EQUAL(s[7],  0x00)

    // Ключ 1 (4 байта, так как это uint32_t)
    ASSERT_EQUAL(s[8],  0x01)
    ASSERT_EQUAL(s[9],  0x00)
    ASSERT_EQUAL(s[10], 0x00)
    ASSERT_EQUAL(s[11], 0x00)

    // Значение: сначала размер строки (size_t)
    ASSERT_EQUAL(s[12], 0x05)
    ASSERT_EQUAL(s[13], 0x00)
    ASSERT_EQUAL(s[14], 0x00)
    ASSERT_EQUAL(s[15], 0x00)
    ASSERT_EQUAL(s[16], 0x00)
    ASSERT_EQUAL(s[17], 0x00)
    ASSERT_EQUAL(s[18], 0x00)
    ASSERT_EQUAL(s[19], 0x00)

    // Теперь сама строка (ASCII-коды):
    ASSERT_EQUAL(s[20], 0x68)  // 'h'
    ASSERT_EQUAL(s[21], 0x65)  // 'e'
    ASSERT_EQUAL(s[22], 0x6c)  // 'l'
    ASSERT_EQUAL(s[23], 0x6c)  // 'l'
    ASSERT_EQUAL(s[24], 0x6f)  // 'o'

    // Ключ 2
    ASSERT_EQUAL(s[25], 0x02)
    ASSERT_EQUAL(s[26], 0x00)
    ASSERT_EQUAL(s[27], 0x00)
    ASSERT_EQUAL(s[28], 0x00)

    // Значение ключа 2: размер
    ASSERT_EQUAL(s[29], 0x03)
    ASSERT_EQUAL(s[30], 0x00)
    ASSERT_EQUAL(s[31], 0x00)
    ASSERT_EQUAL(s[32], 0x00)
    ASSERT_EQUAL(s[33], 0x00)
    ASSERT_EQUAL(s[34], 0x00)
    ASSERT_EQUAL(s[35], 0x00)
    ASSERT_EQUAL(s[36], 0x00)

    // Значение ключа 2: строка
    ASSERT_EQUAL(s[37], 0x62)  // 'b'
    ASSERT_EQUAL(s[38], 0x79)  // 'y'
    ASSERT_EQUAL(s[39], 0x65)  // 'e'

    std::map<uint32_t, std::string> m2;
    Deserialize(ss, m2);

    ASSERT_EQUAL(m, m2)
}


void ComplexTest() {
    const std::vector<std::map<std::string, std::string>> v = {
            {
                    {"hello", "world"},
                    {"foo", "bar"},
            },
            {
                    {"world", "hello"},
                    {"bar", "foo"},
            },
            {
                    {"foo", "foo"},
                    {"bar", "bar"},
            }
    };

    std::stringstream ss;
    Serialize(v, ss);

    std::vector<std::map<std::string, std::string>> v2;
    Deserialize(ss, v2);

    ASSERT_EQUAL(v, v2)
}


void TestSaveLoad() {
    TestRunner tr;
    RUN_TEST(tr, TestSaveAndLoad);
    RUN_TEST(tr, ComplexTest);
}