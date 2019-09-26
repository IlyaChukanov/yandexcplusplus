//
// Created by ilya on 15.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_FINAL_PROJECT_NODE_H_
#define SECONDTWEEK_YELLOW_FINAL_PROJECT_NODE_H_

#include <string>
#include <memory>

#include "date.h"

enum class Comparison {
  Less,
  LessOrEqual,
  Greater,
  GreaterOrEqual,
  Equal,
  NotEqual
};

enum class LogicalOperation {
  Or,
  And
};

template <typename ToCompare>
bool Compare(Comparison cmp, const ToCompare& left, const ToCompare& right) {
  switch (cmp) {
    // TODO Реализовать все операторы для сравнений дат
    case Comparison::Less: {
      return left < right;
    }
    case Comparison ::LessOrEqual: {
      return left <= right;
    }
    case Comparison::Greater: {
      return left > right;
    }
    case Comparison::GreaterOrEqual: {
      return left >= right;
    }
    case Comparison::Equal: {
      return left == right;
    }
    case Comparison ::NotEqual: {
      return left != right;
    }
    default: {
      throw std::logic_error("Unknown comparison token");
    }
  }
}

class Node {
 public:
  virtual bool Evaluate(const Date& date, const std::string& event) = 0;
};

class EmptyNode : public Node {
 public:
  bool Evaluate(const Date& date, const std::string& event) override;
};

class DateComparisonNode : public Node {
 public:
  DateComparisonNode(Comparison cmp, const Date& date);
  bool Evaluate(const Date& date, const std::string& event) override;
 private:
  Comparison cmp_;
  Date date_;
};

class EventComparisonNode : public Node{
 public:
  EventComparisonNode(Comparison cmp, const std::string& event);
  bool Evaluate(const Date& date, const std::string& event) override;
 private:
  Comparison cmp_;
  std::string event_;
};

class LogicalOperationNode : public Node {
 public:
  LogicalOperationNode(LogicalOperation lo, std::shared_ptr<Node> left, std::shared_ptr<Node> right);
  bool Evaluate(const Date& date, const std::string& event) override;
 private:
  LogicalOperation operation_;
  std::shared_ptr<Node> left_, right_;
};

#endif //SECONDTWEEK_YELLOW_FINAL_PROJECT_NODE_H_
