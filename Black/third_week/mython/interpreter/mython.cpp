#include "object.h"
#include "object_holder.h"
#include "statement.h"
#include "lexer.h"
#include "parse.h"
#include "UnitTestsFramework.h"
#include "complex_tests.h"
#include <string>
#include <iostream>


void RunMythonProgram(std::istream &input, std::ostream &output) {
    Ast::Print::SetOutputStream(output);

    Parse::Lexer lexer(input);
    auto program = ParseProgram(lexer);

    Runtime::Closure closure;
    program->Execute(closure);
}


void TestSimplePrints() {
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
    RunMythonProgram(input, output);

    ASSERT_EQUAL(output.str(), "57\n10 24 -8\nhello\nworld\nTrue False\n\nNone\n")
}


void TestAssignments() {
    std::istringstream input(R"(
x = 57
print x
x = 'C++ black belt'
print x
y = False
x = y
print x
x = None
print x, y
)");

    std::ostringstream output;
    RunMythonProgram(input, output);

    ASSERT_EQUAL(output.str(), "57\nC++ black belt\nFalse\nNone False\n")
}


void TestArithmetics() {
    std::istringstream input(
            "print 1+2+3+4+5, 1*2*3*4*5, 1-2-3-4-5, 36/4/3, 2*5+10/2"
    );

    std::ostringstream output;
    RunMythonProgram(input, output);

    ASSERT_EQUAL(output.str(), "15 120 -13 3 15\n")
}


void TestVariablesArePointers() {
    std::istringstream input(R"(
class Counter:
  def __init__():
    self.value = 0

  def add():
    self.value = self.value + 1

class Dummy:
  def do_add(counter):
    counter.add()

x = Counter()
y = x

x.add()
y.add()

print x.value

d = Dummy()
d.do_add(x)

print y.value
)");

    std::ostringstream output;
    RunMythonProgram(input, output);

    ASSERT_EQUAL(output.str(), "2\n3\n")
}


void TestAll() {
    TestRunner tr;

    Runtime::RunObjectHolderTests(tr);
    Runtime::RunObjectsTests(tr);
    Ast::RunUnitTests(tr);
    Parse::RunLexerTests(tr);
    TestParseProgram(tr);
    RUN_TEST(tr, TestSimplePrints);
    RUN_TEST(tr, TestAssignments);
    RUN_TEST(tr, TestArithmetics);
    RUN_TEST(tr, TestVariablesArePointers);
    RunComplexTests(tr);
}


int main() {
    TestAll();

    RunMythonProgram(std::cin, std::cout);

    return 0;
}