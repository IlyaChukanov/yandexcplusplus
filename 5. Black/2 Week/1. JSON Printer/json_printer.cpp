//
// Created by ilya_l on 4/26/20.
//

//#include <MyTestFramework/TestFramework.h>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <stack>
#include <string>

void PrintJsonString(std::ostream& out, std::string_view str) {
  out << "\"";
  for (const auto ch : str) {
    if (ch == '\"' || ch == '\\') {
      out << "\\" << ch;
    }
    else {
      out << ch;
    }
  }
  out << "\"";
}

void PrintJsonInt(std::ostream& out, int64_t num) {
  out << num;
}

void PrintJsonBool(std::ostream& out, bool buf) {
  out << std::boolalpha << buf;
}

void PrintJsonNull(std::ostream& out) {
  out << "null";
}

template <typename Object>
class JSONObject;

template <typename Owner>
class JSONArray {
public:
  JSONArray(std::ostream& out, Owner& parent) : out_(out), parent_object(parent) {
    out << "[";
  }
  ~JSONArray() {
    if (!end) {
      end = true;
      out_ << "]";
    }
  }
  JSONArray& Number(uint64_t num) {
    PrintDeliminator();
    PrintJsonInt(out_, num);
    return *this;
  }
  JSONArray& String(std::string_view str) {
    PrintDeliminator();
    PrintJsonString(out_, str);
    return *this;
  }
  JSONArray& Boolean(bool flag) {
    PrintDeliminator();
    PrintJsonBool(out_, flag);
    return *this;
  }
  JSONArray& Null() {
    PrintDeliminator();
    PrintJsonNull(out_);
    return *this;
  }
  auto BeginArray() {
    PrintDeliminator();
    return JSONArray<JSONArray<Owner>>(out_, *this);
  }
  auto BeginObject() {
    PrintDeliminator();
    return JSONObject<JSONArray<Owner>>(out_, *this);
  }
  Owner& EndArray() {
    end = true;
    out_ << "]";
    return parent_object;
  }
protected:
  void PrintDeliminator() {
    if (start) {
      out_ << ",";
    }
    else {
      start = true;
    }
  }
  std::ostream& out_;
  Owner& parent_object;
  bool start = false, end = false;
};


template <typename Object>
class KeyValueCell {
public:
  KeyValueCell(std::ostream& out, Object& obj) : out_(out), map(obj) {}
  ~KeyValueCell() {
    if (!has_ended) {
      PrintJsonNull(out_);
    }
  }
  Object& Number(uint64_t num) {
    has_ended = true;
    PrintJsonInt(out_, num);
    return map;
  }
  Object& String(std::string_view str) {
    has_ended = true;
    PrintJsonString(out_, str);
    return map;
  }
  Object& Boolean(bool flag) {
    has_ended = true;
    PrintJsonBool(out_, flag);
    return map;
  }
  Object& Null() {
    has_ended = true;
    PrintJsonNull(out_);
    return map;
  }
  auto BeginArray() {
    has_ended = true;
    return JSONArray(out_, map);
  }
  auto BeginObject() {
    has_ended = true;
    return JSONObject(out_, map);
  }
private:
  bool has_ended = false;
  Object& map;
  std::ostream& out_;
};

template <typename Owner>
class JSONObject {
public:
  JSONObject(std::ostream& out, Owner& parent) : out_(out), parent_object(parent) {
    out_ << "{";
  }
  ~JSONObject() {
    if (!end) {
      out_ << "}";
    }
  }

  auto Key(std::string_view key) {
    PrintDeliminator();
    PrintJsonString(out_, key);
    out_ << ":";
    return KeyValueCell(out_, *this);
  }

  Owner& EndObject() {
    end = true;
    out_ << "}";
    return parent_object;
  }
protected:
  void PrintDeliminator() {
    if (start) {
      out_ << ",";
    }
    else {
      start = true;
    }
  }
  Owner& parent_object;
  std::ostream& out_;
  bool start = false, end = false;
};

class EmptyContext {
public:
  EmptyContext() = default;
};

using ArrayContext = JSONArray<EmptyContext>;  // Замените это объявление на определение типа ArrayContext
ArrayContext PrintJsonArray(std::ostream& out) {
  EmptyContext c;
  return JSONArray(out, c);
}

using ObjectContext = JSONObject<EmptyContext>;  // Замените это объявление на определение типа ObjectContext
ObjectContext PrintJsonObject(std::ostream& out) {
  EmptyContext c;
  return JSONObject(out, c);
}

/*
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

  ASSERT_EQUAL(output.str(), R"([5,6,[7],8,"bingo!"])");
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

  ASSERT_EQUAL(output.str(), R"({"id1":1234,"id2":false,"":null,"\"":"\\"})");
}

void TestAutoClose() {
  std::ostringstream output;

  {
    auto json = PrintJsonArray(output);
    json.BeginArray().BeginObject();
  }

  ASSERT_EQUAL(output.str(), R"([[{}]])");
}

void TestNullKey() {
  std::ostringstream output;

  {
    auto json = PrintJsonObject(output);
    json.Key("foo")
        .BeginArray()
        .String("Hello")
        .EndArray()
        .Key("foo")  // повторяющиеся ключи допускаются
        .BeginObject()
        .Key("foo");
  }
  ASSERT_EQUAL(output.str(), R"({"foo":["Hello"],"foo":{"foo":null}})");
}

void TestHelloArray() {
  std::ostringstream output;

  {
    auto json = PrintJsonArray(output);
    json
        .String("Hello")
        .BeginArray()
        .String("World");
  }

  ASSERT_EQUAL(output.str(), R"(["Hello",["World"]])");
}

void TestArray1() {
  std::ostringstream output;

  {
    auto json = PrintJsonArray(output);
    json
        .Null()
        .String("Hello")
        .Number(123)
        .Boolean(false)
        .EndArray();
  }

  ASSERT_EQUAL(output.str(), R"([null,"Hello",123,false])");
}

void TestArray2() {
  std::ostringstream output;

  {
    auto json = PrintJsonArray(output);
    json
        .Null()
        .String("Hello")
        .Number(123)
        .Boolean(false);
  }

  ASSERT_EQUAL(output.str(), R"([null,"Hello",123,false])");
}
*/

#include <iostream>

int main() {
  //TestRunner tr;
//  RUN_TEST(tr, TestArray);
//  RUN_TEST(tr, TestObject);
//  RUN_TEST(tr, TestAutoClose);
//  RUN_TEST(tr, TestHelloArray);
//  RUN_TEST(tr, TestNullKey);
//  RUN_TEST(tr, TestArray1);
//  RUN_TEST(tr, TestArray2);

  auto json = PrintJsonArray(std::cout);
  json
      .String("hello")
      .Null()
      .BeginArray()
      .Number(10)
      .BeginObject()
      .EndObject()
      .String("")
      .Null()
      .BeginArray();
  return 0;
}

