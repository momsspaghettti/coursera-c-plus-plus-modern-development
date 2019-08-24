#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <sstream>


template <typename First, typename Second>
std::ostream& operator << (std::ostream& out, const std::pair<First, Second>& p);

template <typename Collection>
std::string Join(const Collection& c, const std::string& d);

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vi);

template <typename Key, typename Value>
std::ostream& operator << (std::ostream& out, const std::map<Key, Value>& m);

template <typename Key, typename Value>
std::ostream& operator << (std::ostream& out, const std::unordered_map<Key, Value>& m);

template <typename T>
std::ostream& operator << (std::ostream& out, const std::set<T>& s);

template<class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint);

void Assert(bool cond, const std::string& hint);


class TestRunner
{
public:
	TestRunner();
	~TestRunner();
	template<class TestFunction>
	void RunTest(TestFunction test_function, const std::string& test_function_name);

private:
	int fail_count;
};


template <typename First, typename Second>
std::ostream& operator << (std::ostream& out, const std::pair<First, Second>& p)
{
	return out << '(' << p.first << ", " << p.second << ')';
}


template <typename Collection>
std::string Join(const Collection& c, const std::string& d)
{
	std::stringstream ss;
	bool first = true;
	for (const auto& i : c) {
		if (!first) {
			ss << d;
		}
		first = false;
		ss << i;
	}
	return ss.str();
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vi)
{
	return out << '[' << Join(vi, ", ") << ']';
}

template <typename Key, typename Value>
std::ostream& operator << (std::ostream& out, const std::map<Key, Value>& m)
{
	return out << '{' << Join(m, ", ") << '}';
}


template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<Key, Value>& m)
{
	return out << '{' << Join(m, ", ") << '}';
}


template <typename T>
std::ostream& operator << (std::ostream& out, const std::set<T>& s)
{
	return out << '{' << Join(s, ", ") << '}';
}


template<class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint)
{
	if (t != u)
	{
		std::ostringstream os;
		os << "Assertion failed: " << t << " != " << u << " Hint: " << hint;

		throw std::runtime_error(os.str());
	}
}


template<class TestFunction>
void TestRunner::RunTest(TestFunction test_function, const std::string& test_function_name)
{

	try
	{
		test_function();
		std::cerr << test_function_name << " OK" << std::endl;
	}
	catch (std::runtime_error& ex)
	{
		++fail_count;
		std::cerr << test_function_name << " fail: " << ex.what() << std::endl;
	}
}


#define ASSERT_EQUAL(x, y)              \
{                                       \
  std::ostringstream os;                \
  os << #x << " != " << #y << ", "      \
    << __FILE__ << ":" << __LINE__;     \
  AssertEqual(x, y, os.str());          \
}

#define ASSERT(x)                       \
{                                       \
  std::ostringstream os;                \
  os << #x << " is false, "             \
    << __FILE__ << ":" << __LINE__;     \
  Assert(x, os.str());                  \
}

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)