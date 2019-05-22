#pragma once
#include <tuple>
#include <map>
#include <set>
#include <utility>
#include <sstream>
#include <iostream>
#include <vector>


using namespace std;


template<typename T> T Sqr(const T& object);
template<typename T> vector<T> operator*(const vector<T>& vec1, const vector<T>& vec2);
template<typename Key, typename Value> map<Key, Value> operator*(const map<Key, Value>& map1, const map<Key, Value>& map2);
template<typename First, typename Second> pair<First, Second> operator*(const pair<First, Second>& pair1, const pair<First, Second>& pair2);


template<typename T> vector<T> operator*(const vector<T>& vec1, const vector<T>& vec2)
{
	vector<T> result;

	for (size_t i = 0; i < vec1.size(); ++i)
	{
		result.push_back(Sqr(vec1[i]));
	}

	return result;
}


template<typename Key, typename Value> map<Key, Value> operator*(const map<Key, Value>& map1, const map<Key, Value>& map2)
{
	map<Key, Value> result;

	for (const auto& pair_ : map1)
	{
		result[pair_.first] = Sqr(pair_.second);
	}

	return result;
}


template<typename First, typename Second> pair<First, Second> operator*(const pair<First, Second>& pair1, const pair<First, Second>& pair2)
{
	return make_pair(Sqr(pair1.first), Sqr(pair1.second));
}


template<typename T> T Sqr(const T& object)
{
	return object * object;
}


template <typename Collection>
string Join(const Collection& c, char d) {
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

template <typename First, typename Second>
ostream& operator << (ostream& out, const pair<First, Second>& p) {
	return out << '(' << p.first << ',' << p.second << ')';
}

template <typename T>
ostream& operator << (ostream& out, const vector<T>& vi) {
	return out << '[' << Join(vi, ',') << ']';
}

template <typename Key, typename Value>
ostream& operator << (ostream& out, const map<Key, Value>& m) {
	return out << '{' << Join(m, ',') << '}';
}


template <typename Key, typename Value> Value& GetRefStrict(map<Key, Value>& map_, const Key& key);


template <typename Key, typename Value> Value& GetRefStrict(map<Key, Value>& map_, const Key& key)
{
	try
	{
		return map_.at(key);
	}
	catch (out_of_range&)
	{
		throw runtime_error("Index out of range!");
	}
}
