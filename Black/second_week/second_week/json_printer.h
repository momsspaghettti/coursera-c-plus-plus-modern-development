#pragma once
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <optional>
#include "UnitTestsFramework.h"


class EmptyContext
{

};


void PrintJsonString(std::ostream& out, std::string_view str);


class JsonObject
{
public:
    explicit JsonObject(std::ostream& o) : out(o) {}

    std::ostream& out;

    virtual ~JsonObject() = default;
};


class JsonNumber : public JsonObject
{
public:
    JsonNumber(std::ostream& o, int64_t n) : JsonObject(o), number_(n) {}

    ~JsonNumber() override {
        out << number_;
    }

private:
    int64_t number_;
};


class JsonString : public JsonObject
{
public:
    JsonString(std::ostream& o, std::string_view s) : JsonObject(o), s_(s) {}

    ~JsonString() override {
        PrintJsonString(out, s_);
    }

private:
    std::string s_;
};


class JsonBoolean : public JsonObject
{
public:
    JsonBoolean(std::ostream& o, bool f) : JsonObject(o), flag_(f) {}

    ~JsonBoolean() override {
        if (flag_) {
            out << "true";
        } else {
            out << "false";
        }
    }

private:
    bool flag_;
};


class JsonNull : public JsonObject
{
public:
    explicit JsonNull(std::ostream& o) : JsonObject(o) {}

    ~JsonNull() override {
        out << "null";
    }
};


class JsonMap : public JsonObject
{
public:
    explicit JsonMap(std::ostream& o) : JsonObject(o) {}

    std::pair<std::string, JsonObject*>& AddKey(const std::string& key) {
        auto* value = new JsonNull(out);
        data_.emplace_back(std::make_pair(key, value));
        return *data_.rbegin();
    }

    ~JsonMap() override {
        out << "{";
        bool first = true;
        for (const auto& [key, value] : data_) {
            if (first) {
                PrintJsonString(out, key);
                out << ":";
                delete value;
                first = false;
            } else {
                out << ",";
                PrintJsonString(out, key);
                out << ":";
                delete value;
            }
        }
        out << "}";
    }

private:
    std::vector<std::pair<std::string, JsonObject*>> data_;
};


class JsonArray : public JsonObject
{
public:
    explicit JsonArray(std::ostream& o) : JsonObject(o) {}

    void Add(JsonObject* obj) {
        data_.push_back(obj);
    }

    ~JsonArray() override {
        bool first = true;
        out << "[";
        for (const auto pointer : data_) {
            if (first) {
                delete pointer;
                first = false;
            } else {
                out << ",";
                delete pointer;
            }
        }
        out << "]";
    }

private:
    std::vector<JsonObject*> data_;
};


template <class Parent>
class InMapKeyOrEndExpected;
template <class Parent>
class InMapValueExpected;


template <class Parent>
class InArray : public EmptyContext
{
public:
    InArray(std::ostream& out, Parent* p) : out_(out) {
        array_ = new JsonArray(out_);
        parent_ = p;
    }

    InArray(std::ostream& out, JsonArray* a, Parent* p) : out_(out) {
        array_ = a;
        parent_ = p;
    }

    InArray& Number(int64_t n) {
        array_->Add(new JsonNumber(out_, n));
        return *this;
    }

    InArray& String(std::string_view s) {
        array_->Add(new JsonString(out_, s));
        return *this;
    }

    InArray& Boolean(bool f) {
        array_->Add(new JsonBoolean(out_, f));
        return *this;
    }

    InArray& Null() {
        array_->Add(new JsonNull(out_));
        return *this;
    }

    InArray<InArray> BeginArray() {
        auto* a = new JsonArray(out_);
        array_->Add(a);
        return InArray<InArray>(out_, a, this);
    }

    InMapKeyOrEndExpected<InArray> BeginObject() {
        auto* m = new JsonMap(out_);
        array_->Add(m);
        return InMapKeyOrEndExpected<InArray>(out_, this, m);
    }

    Parent& EndArray() {
        return *parent_;
    }

    ~InArray() {
        if (typeid(parent_) == typeid(EmptyContext*)) {
            delete array_;
            delete parent_;
        }
    }

private:
    std::ostream& out_;
    JsonArray* array_;
    Parent* parent_;
};


template <class Parent>
class InMapKeyOrEndExpected : public EmptyContext
{
public:
    InMapKeyOrEndExpected(std::ostream& out, Parent* p) : out_(out) {
        parent_ = p;
        map_ = new JsonMap(out_);
    }

    InMapKeyOrEndExpected(std::ostream& out, Parent* p, JsonMap* m) : out_(out) {
        parent_ = p;
        map_ = m;
    }

    InMapValueExpected<InMapKeyOrEndExpected> Key(std::string_view key) {
        std::pair<std::string, JsonObject*>& value = map_->AddKey(std::string(key));
        return InMapValueExpected<InMapKeyOrEndExpected>(out_, this, value);
    }

    Parent& EndObject() {
        return *parent_;
    }

    ~InMapKeyOrEndExpected() {
        if (typeid(parent_) == typeid(EmptyContext*)) {
            delete map_;
            delete parent_;
        }
    }

private:
    std::ostream& out_;
    Parent* parent_;
    JsonMap* map_;
};


template <class Parent>
class InMapValueExpected : public EmptyContext
{
public:
    InMapValueExpected(std::ostream& out, Parent* p, std::pair<std::string, JsonObject*>& obj) : out_(out),
    value_(obj) {
        parent_ = p;
    }

    Parent& Number(int64_t n) {
        value_.second = new JsonNumber(out_, n);
        return *parent_;
    }

    Parent& String(std::string_view s) {
        value_.second = new JsonString(out_, s);
        return *parent_;
    }

    Parent& Boolean(bool f) {
        value_.second = new JsonBoolean(out_, f);
        return *parent_;
    }

    Parent& Null() {
        return *parent_;
    }

    auto BeginArray() {
        auto* array = new JsonArray(out_);
        value_.second = array;
        return InArray(out_, array, parent_);
    }

    auto BeginObject() {
        auto* map = new JsonMap(out_);
        value_.second = map;
        return InMapKeyOrEndExpected<Parent>(out_, parent_, map);
    }

private:
    std::ostream& out_;
    std::pair<std::string, JsonObject*>& value_;
    Parent* parent_;
};

using ArrayContext = InArray<EmptyContext>;  // Замените это объявление на определение типа ArrayContext
ArrayContext PrintJsonArray(std::ostream& out);


using ObjectContext = InMapKeyOrEndExpected<EmptyContext>;  // Замените это объявление на определение типа ObjectContext
ObjectContext PrintJsonObject(std::ostream& out);


void TestJsonPrinter();