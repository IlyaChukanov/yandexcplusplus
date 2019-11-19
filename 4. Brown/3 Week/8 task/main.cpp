#include "Common.h"
#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include <sstream>

using namespace std;

string Print(const Expression* e) {
  if (!e) {
    return "Null expression provided";
  }
  stringstream output;
  output << e->ToString() << " = " << e->Evaluate();
  return output.str();
}

namespace Expressions {
class Value : public Expression {
 public:

  explicit Value(int value) : value_(value) {}
  int Evaluate() const override {
    return value_;
  }

  std::string ToString() const override {
    return std::to_string(value_);
  }
 private:
  int value_;
};

class Sum : public Expression {
 public:
  Sum(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) : left_(std::move(left)),
                                                                             right_(std::move(right)) {}
  int Evaluate() const override {
    return left_->Evaluate() + right_->Evaluate();
  }

  std::string ToString() const override {
    return "(" + left_->ToString() + ")+(" + right_->ToString() + ")";
  }
 private:
  std::unique_ptr<Expression> left_, right_;
};

class Product : public Expression {
 public:
  Product(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) : left_(std::move(left)),
                                                                                 right_(std::move(right)) {}
  int Evaluate() const override {
    return left_->Evaluate() * right_->Evaluate();
  }

  std::string ToString() const override {
    return "(" + left_->ToString() + ")*(" + right_->ToString() + ")";
  }
 private:
  std::unique_ptr<Expression> left_, right_;
};
}

ExpressionPtr Value(int value) {
  return std::make_unique<Expressions::Value>(Expressions::Value(value));
}
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
  return std::make_unique<Expressions::Sum>(Expressions::Sum(std::move(left), std::move(right)));
}
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
  return std::make_unique<Expressions::Product>(Expressions::Product(std::move(left), std::move(right)));
}

void Test() {
  ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
  ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

  ExpressionPtr e2 = Sum(move(e1), Value(5));
  ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

  ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}