#pragma once
#include <map>
#include <iostream>
#include <string>
#include <vector>


// Serialization
template <typename T>
void Serialize(T pod, std::ostream& out);

void Serialize(const std::string& str, std::ostream& out);

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out);

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out);


template <typename T>
void Serialize(T pod, std::ostream& out) {
    out.write(
            reinterpret_cast<const char*>(&pod),
            sizeof(pod)
            );
}


inline void Serialize(const std::string& str, std::ostream& out) {
    size_t size_ = str.size();
    out.write(
            reinterpret_cast<const char*>(&size_),
            sizeof(size_)
    );
    for (const auto& ch : str) {
        Serialize(ch, out);
    }
}


template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
    size_t size_ = data.size();
    out.write(
            reinterpret_cast<const char*>(&size_),
            sizeof(size_)
            );
    for (const auto& item : data) {
        Serialize(item, out);
    }
}


template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
    size_t size_ = data.size();
    out.write(
            reinterpret_cast<const char*>(&size_),
            sizeof(size_)
            );
    for (const auto& [key, value] : data) {
        Serialize(key, out);
        Serialize(value, out);
    }
}


// Deserialization
template <typename T>
void Deserialize(std::istream& in, T& pod);

void Deserialize(std::istream& in, std::string& str);

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data);

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data);


template <typename T>
void Deserialize(std::istream& in, T& pod) {
    in.read(
            reinterpret_cast<char*>(&pod),
            sizeof(pod)
            );
}


inline void Deserialize(std::istream& in, std::string& str) {
    size_t size_;
    in.read(
            reinterpret_cast<char*>(&size_),
            sizeof(size_)
            );
    str = std::string(size_, ' ');
    for (auto& ch : str) {
        Deserialize(in, ch);
    }
}


template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
    size_t size_;
    in.read(
            reinterpret_cast<char*>(&size_),
            sizeof(size_)
    );
    data = std::vector<T>(size_);
    for (auto& item : data) {
        Deserialize(in, item);
    }
}


template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
    size_t size_;
    in.read(
            reinterpret_cast<char*>(&size_),
            sizeof(size_)
    );
    for (size_t i = 0; i < size_; ++i) {
        T1 key;
        Deserialize(in, key);
        T2 value;
        Deserialize(in, value);
        data[key] = value;
    }
}


void TestSaveLoad();