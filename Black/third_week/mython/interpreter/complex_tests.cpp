#include "complex_tests.h"


void RunProgram(std::istream &input, std::ostream &output) {
    Ast::Print::SetOutputStream(output);

    Parse::Lexer lexer(input);
    auto program = ParseProgram(lexer);

    Runtime::Closure closure;
    program->Execute(closure);
}


void TestEmptyProgram() {
    std::istringstream input(R"()");

    std::ostringstream output;
    RunProgram(input, output);

    ASSERT_EQUAL(output.str(), "")
}


void TestPrinting() {
    std::istringstream input(R"(
print 57
print 10, 24, -8
print 'hello'
print "world"
print True, False
print
print None
)");

    std::ostringstream output;
    RunProgram(input, output);

    ASSERT_EQUAL(output.str(), "57\n10 24 -8\nhello\nworld\nTrue False\n\nNone\n")
}


void TestStrFunction() {
    std::istringstream input(R"(
x = 1
xx = 2
_xxx = 3
x4 = 4
x_5 = 5

str_ = 'string'
str_2 = str_
str_3 = str_2

print str((x + xx) / _xxx) + str(x4) + str(x_5 + 1) + str_ + str_2 + str_3
)");

    std::ostringstream output;
    RunProgram(input, output);

    ASSERT_EQUAL(output.str(), "146stringstringstring\n")
}


void TestTwoLongStrings() {
    std::istringstream input(R"(
string1 = 'Here is "test string" with inner "double""quotes"'
string2 = "Here is another 'test string' with inner 'single''quotes'"

print string1
print string2
)");

    std::ostringstream output;
    RunProgram(input, output);

    ASSERT_EQUAL(output.str(), "Here is \"test string\" with inner \"double\"\"quotes\"\n"
                               "Here is another 'test string' with inner 'single''quotes'\n")
}


void TestSimpleClassAbs() {
    std::istringstream input(R"(
class Abs:
  def calc(n):
    if n > 0:
      return n
    else:
      return -n

x = Abs()

print x.calc(2), x.calc(-3), x.calc(0)
)");

    std::ostringstream output;
    RunProgram(input, output);

    ASSERT_EQUAL(output.str(), "2 3 0\n")
}


void TestBoolOperationsOnTypes() {
    std::istringstream input(R"(
a = 1
b = 2
c = 3

result1 = a + b > c and a + c > b and b + c > a

a = False
b = False
c = True

result2 = not a and b or c
result3 = not a and (b or c)
result4 = not(not a and (b or c))

a = 'this'
b = 'is'
c = 'test'

result5 = a > b and a > c and c > b
result6 = a < b or a < c or c < b

a = ''
b = ""
c = 0

result7 = a > b
result8 = a or b or c

a = 0
b = 100
c = ''

result9 = a or b or c

a = None
b = None

result10 = a and b
result11 = not a or b

a = 1
b = 1
c = 2

result12 = a == b and a != c

a = '1'
b = '1'
c = "2"

result13 = a == b and a != c

print result1, result2, result3, result4, result5, result6, result7, result8, result9, result10, result11, result12, result13
)");

    std::ostringstream output;
    RunProgram(input, output);

    ASSERT_EQUAL(output.str(), "False True True False True False False False True False True True True\n")
}


void RunComplexTests(TestRunner& tr) {
    RUN_TEST(tr, TestEmptyProgram);
    RUN_TEST(tr, TestPrinting);
    RUN_TEST(tr, TestStrFunction);
    RUN_TEST(tr, TestTwoLongStrings);
    RUN_TEST(tr, TestSimpleClassAbs);
    RUN_TEST(tr, TestBoolOperationsOnTypes);
}