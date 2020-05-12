//
// Created by ilya_l on 5/10/20.
//

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <stack>
#include <string>


void PrintJsonString(std::ostream& out, std::string_view str) {
  out << '"';
  for (unsigned char c : str) {
    if (c == '\\') {
      out << "\\\\";
    } else if (c == '"') {
      out << "\\\"";
    } else {
      out << c;
    }
  }
  out << '"';
}

template <typename Parent>
class ContextBase {
protected:
  ContextBase(Parent& parent, std::ostream& output)
      : parent_(parent)
      , output_(output)
  {
  }

  Parent& parent_;
  std::ostream& output_;
};

template <typename Parent>
class ObjectContext;

template <typename Parent>
class ArrayContext;

template <typename Parent>
class ValueContext : public ContextBase<Parent> {
public:
  ValueContext(Parent& parent, std::ostream& output)
      : ContextBase<Parent>(parent, output)
  {
  }

  ~ValueContext() {
    if (!wrote_value_) {
      Null();
    }
  }

  Parent& Number(int64_t value) {
    wrote_value_ = true;
    this->output_ << value;
    return this->parent_;
  }

  Parent& String(std::string_view value) {
    wrote_value_ = true;
    PrintJsonString(this->output_, value);
    return this->parent_;
  }

  Parent& Boolean(bool value) {
    wrote_value_ = true;
    this->output_ << (value ? "true" : "false");
    return this->parent_;
  }

  Parent& Null() {
    wrote_value_ = true;
    this->output_ << "null";
    return this->parent_;
  }

  ArrayContext<Parent> BeginArray();
  ObjectContext<Parent> BeginObject();

private:
  bool wrote_value_ = false;
};

template <typename Parent>
class ObjectContext : public ContextBase<Parent> {
public:
  using Self = ObjectContext<Parent>;

  ObjectContext(Parent& parent, std::ostream& output) : ContextBase<Parent>(parent, output) {
    this->output_ << '{';
  }

  ~ObjectContext() {
    if (!finished_) {
      EndObject();
    }
  }

  Parent& EndObject() {
    this->output_ << '}';
    finished_ = true;
    return this->parent_;
  }

  ValueContext<Self> Key(std::string_view key) {
    if (!first_) {
      this->output_ << ',';
    }
    first_ = false;
    PrintJsonString(this->output_, key);
    this->output_ << ':';

    return {*this, this->output_};
  }

private:
  bool first_ = true;
  bool finished_ = false;
};

template <typename Parent>
class ArrayContext : public ContextBase<Parent> {
public:
  using Self = ArrayContext<Parent>;

  ArrayContext(Parent& parent, std::ostream& output)
      : ContextBase<Parent>(parent, output)
  {
    this->output_ << '[';
  }

  ~ArrayContext() {
    if (!finished_) {
      EndArray();
    }
  }

  Self& Number(int64_t value) {
    PutComma();
    this->output_ << value;
    return *this;
  }

  Self& String(std::string_view value) {
    PutComma();
    PrintJsonString(this->output_, value);
    return *this;
  }

  Self& Boolean(bool value) {
    PutComma();
    this->output_ << (value ? "true" : "false");
    return *this;
  }

  Self& Null() {
    PutComma();
    this->output_ << "null";
    return *this;
  }

  ArrayContext<Self> BeginArray();

  ObjectContext<Self> BeginObject() {
    PutComma();
    return {*this, this->output_};
  }

  Parent& EndArray() {
    this->output_ << ']';
    finished_ = true;
    return this->parent_;
  }

private:
  void PutComma() {
    if (!first_) {
      this->output_ << ',';
    }
    first_ = false;
  }

  bool first_ = true;
  bool finished_ = false;
};

template <typename Parent>
ArrayContext<Parent> ValueContext<Parent>::BeginArray() {
  wrote_value_ = true;
  return {this->parent_, this->output_};
}

template <typename Parent>
ObjectContext<Parent> ValueContext<Parent>::BeginObject() {
  wrote_value_ = true;
  return {this->parent_, this->output_};
}

template <typename Parent>
ArrayContext<ArrayContext<Parent>> ArrayContext<Parent>::BeginArray() {
  PutComma();
  return {*this, this->output_};
}

struct Empty {};

ArrayContext<Empty> PrintJsonArray(std::ostream& out) {
  static Empty empty;
  return {empty, out};
}

ObjectContext<Empty> PrintJsonObject(std::ostream& out) {
  static Empty empty;
  return {empty, out};
}
