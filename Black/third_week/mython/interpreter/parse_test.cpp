#include "parse.h"
#include "lexer.h"
#include "statement.h"
#include "UnitTestsFramework.h"
#include <string>
#include <sstream>

using namespace std;

namespace Parse {

    unique_ptr<Ast::Statement> ParseProgramFromString(const string &program) {
        istringstream is(program);
        Parse::Lexer lexer(is);
        return ParseProgram(lexer);
    }

    void TestSimpleProgram() {
        const string program = R"(
x = 4
y = 5
z = "hello, "
n = "world"
print x + y, z + n
)";

        ostringstream os_;
        Ast::Print::SetOutputStream(os_);

        Runtime::Closure closure;
        auto tree = ParseProgramFromString(program);
        tree->Execute(closure);

        ASSERT_EQUAL(os_.str(), "9 hello, world\n")
    }

    void TestProgramWithClasses() {
        const string program = R"(
program_name = "Classes test"

class Empty:
  def __init__():
    x = 0

class Point:
  def __init__(x, y):
    self.x = x
    self.y = y

  def SetX(value):
    self.x = value
  def SetY(value):
    self.y = value

  def __str__():
    return '(' + str(self.x) + '; ' + str(self.y) + ')'

origin = Empty()
origin = Point(0, 0)

far_far_away = Point(10000, 50000)

print program_name, origin, far_far_away, origin.SetX(1)
)";

        ostringstream os_;
        Ast::Print::SetOutputStream(os_);

        Runtime::Closure closure;
        auto tree = ParseProgramFromString(program);
        tree->Execute(closure);

        ASSERT_EQUAL(os_.str(), "Classes test (0; 0) (10000; 50000) None\n")
    }

    void TestProgramWithIf() {
        const string program = R"(
x = 4
y = 5
if x > y:
  print "x > y"
else:
  print "x <= y"
if x > 0:
  if y < 0:
    print "y < 0"
  else:
    print "y >= 0"
else:
  print 'x <= 0'
)";

        ostringstream os_;
        Ast::Print::SetOutputStream(os_);

        Runtime::Closure closure;
        auto tree = ParseProgramFromString(program);
        tree->Execute(closure);

        ASSERT_EQUAL(os_.str(), "x <= y\ny >= 0\n")
    }

    void TestReturnFromIf() {
        const string program = R"(
class Abs:
  def calc(n):
    if n > 0:
      return n
    else:
      return -n

x = Abs()
print x.calc(2)
)";

        ostringstream os_;
        Ast::Print::SetOutputStream(os_);

        Runtime::Closure closure;
        auto tree = ParseProgramFromString(program);
        tree->Execute(closure);

        ASSERT_EQUAL(os_.str(), "2\n")
    }

    void TestRecursion() {
        const string program = R"(
class ArithmeticProgression:
  def calc(n):
    self.result = 0
    self.calc_impl(n)

  def calc_impl(n):
    value = n
    if value > 0:
      self.result = self.result + value
      self.calc_impl(value - 1)

x = ArithmeticProgression()
x.calc(10)
print x.result
)";

        ostringstream os_;
        Ast::Print::SetOutputStream(os_);

        Runtime::Closure closure;
        auto tree = ParseProgramFromString(program);
        tree->Execute(closure);

        ASSERT_EQUAL(os_.str(), "55\n")
    }

    void TestRecursion2() {
        const string program = R"(
class GCD:
  def __init__():
    self.call_count = 0

  def calc(a, b):
    self.call_count = self.call_count + 1
    if a < b:
      return self.calc(b, a)
    if b == 0:
      return a
    return self.calc(a - b, b)

x = GCD()
print x.calc(510510, 18629977)
print x.calc(22, 17)
print x.call_count
)";

        ostringstream os;
        Ast::Print::SetOutputStream(os);

        Runtime::Closure closure;
        auto tree = ParseProgramFromString(program);
        tree->Execute(closure);

        ASSERT_EQUAL(os.str(), "17\n1\n115\n")
    }

    void TestComplexLogicalExpression() {
        const string program = R"(
a = 1
b = 2
c = 3
ok = a + b > c and a + c > b and b + c > a
print ok
)";

        ostringstream os;
        Ast::Print::SetOutputStream(os);

        Runtime::Closure closure;
        auto tree = ParseProgramFromString(program);
        tree->Execute(closure);

        ASSERT_EQUAL(os.str(), "False\n")
    }

    void TestClassicalPolymorphism() {
        const string program = R"(
class Shape:
  def __str__():
    return "Shape"

class Rect(Shape):
  def __init__(w, h):
    self.w = w
    self.h = h

  def __str__():
    return "Rect(" + str(self.w) + 'x' + str(self.h) + ')'

class Circle(Shape):
  def __init__(r):
    self.r = r

  def __str__():
    return 'Circle(' + str(self.r) + ')'

class Triangle(Shape):
  def __init__(a, b, c):
    self.ok = a + b > c and a + c > b and b + c > a
    if (self.ok):
      self.a = a
      self.b = b
      self.c = c

  def __str__():
    if self.ok:
      return 'Triangle(' + str(self.a) + ', ' + str(self.b) + ', ' + str(self.c) + ')'
    else:
      return 'Wrong triangle'

r = Rect(10, 20)
c = Circle(52)
t1 = Triangle(3, 4, 5)
t2 = Triangle(125, 1, 2)

print r, c, t1, t2
)";

        ostringstream os_;
        Ast::Print::SetOutputStream(os_);

        Runtime::Closure closure;
        auto tree = ParseProgramFromString(program);
        tree->Execute(closure);

        ASSERT_EQUAL(os_.str(), "Rect(10x20) Circle(52) Triangle(3, 4, 5) Wrong triangle\n")
    }


}

void TestParseProgram(TestRunner &tr) {
    RUN_TEST(tr, Parse::TestSimpleProgram);
    RUN_TEST(tr, Parse::TestProgramWithClasses);
    RUN_TEST(tr, Parse::TestProgramWithIf);
    RUN_TEST(tr, Parse::TestReturnFromIf);
    RUN_TEST(tr, Parse::TestRecursion);
    RUN_TEST(tr, Parse::TestRecursion2);
    RUN_TEST(tr, Parse::TestComplexLogicalExpression);
    RUN_TEST(tr, Parse::TestClassicalPolymorphism);
}