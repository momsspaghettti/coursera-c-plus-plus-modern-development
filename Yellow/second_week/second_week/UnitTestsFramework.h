#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>


using namespace std;


template <typename First, typename Second>
ostream& operator << (ostream& out, const pair<First, Second>& p)
{
	return out << '(' << p.first << ", " << p.second << ')';
}

template <typename Collection>
string Join(const Collection& c, const string& d)
{
	stringstream ss;
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
ostream& operator << (ostream& out, const vector<T>& vi)
{
	return out << '[' << Join(vi, ", ") << ']';
}

template <typename Key, typename Value>
ostream& operator << (ostream& out, const map<Key, Value>& m)
{
	return out << '{' << Join(m, ", ") << '}';
}


template <typename T>
ostream& operator << (ostream& out, const set<T>& s)
{
	return out << '{' << Join(s, ", ") << '}';
}


template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint="")
{
	if (t != u)
	{
		ostringstream os;
		os << "Assertion failed: " << t << " != " << u << " Hint: " << hint;

		throw runtime_error(os.str());
	}
}

inline void Assert(bool cond, const string& hint="")
{
	AssertEqual(cond, true, hint);
}


class TestRunner
{
public:
	TestRunner()
    {
		fail_count = 0;
    }

    ~TestRunner()
    {
        if (fail_count > 0)
        {
			cerr << fail_count << " tests failed. Terminate";
			exit(1);
        }
    }

    template<class TestFunction>
    void RunTest(TestFunction test_function, const string& test_function_name="")
	{
        
		try
		{
			test_function();
			cerr << test_function_name << " OK" << endl;
		}
		catch (runtime_error& ex)
		{
			++fail_count;
			cerr << test_function_name << " fail: " << ex.what() << endl;
		}
	}

private:
	int fail_count;
};