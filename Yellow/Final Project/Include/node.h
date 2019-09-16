//
// Created by ilya on 15.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_FINAL_PROJECT_NODE_H_
#define SECONDTWEEK_YELLOW_FINAL_PROJECT_NODE_H_

#include <string>

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

class Node {
 public:
  virtual bool Evaluate(const Date& date, const std::string& event);
};

class EmptyNode : public Node {

};

class DateComparisonNode : public Node {

};

class EventComparisonNode : public Node{

};

class LogicalOperationNode : public Node {

};

#endif //SECONDTWEEK_YELLOW_FINAL_PROJECT_NODE_H_
