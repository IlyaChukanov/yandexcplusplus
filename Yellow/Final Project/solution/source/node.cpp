//
// Created by ilya on 15.09.2019.
//

#include "node.h"

bool EmptyNode::Evaluate(const Date &date, const std::string &event) {
  std::stringstream s;
  s << "Evaluate empty node. " << "Date: " << date.GetDate() << " Event: " << event;
  throw std::logic_error(s.str());
}

DateComparisonNode::DateComparisonNode(Comparison cmp, const Date &date)
    : cmp_(cmp), date_(date) {}

bool DateComparisonNode::Evaluate(const Date &date, const std::string &event) {
  return Compare(cmp_, date_, date);
}

EventComparisonNode::EventComparisonNode(Comparison cmp, const std::string &event)
    : cmp_(cmp), event_(event) {}

bool EventComparisonNode::Evaluate(const Date &date, const std::string &event) {
  return Compare(cmp_, event_, event);
}

LogicalOperationNode::LogicalOperationNode(LogicalOperation lo,
                                           std::shared_ptr<Node> left,
                                           std::shared_ptr<Node> right)
    : operation_(lo), left_(std::move(left)), right_(std::move(right)) {}

bool LogicalOperationNode::Evaluate(const Date &date, const std::string &event) {
  if (operation_ == LogicalOperation::And) {
    return left_->Evaluate(date, event) && right_->Evaluate(date, event);
  }
  else if (operation_ == LogicalOperation::Or) {
    return left_->Evaluate(date, event) || right_->Evaluate(date, event);
  }
  else {
    return false;
  }
}