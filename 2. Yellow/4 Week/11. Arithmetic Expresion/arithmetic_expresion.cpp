//
// Created by ilya on 12.09.2019.
//

#include <sstream>

#include "ArithmeticExpresion.h"

std::istream& operator>>(std::istream& stream, Operation& op) {
  stream >> op.operation >> op.number;
}

ArithmeticExpresion::ArithmeticExpresion(int start_number) {
  numbers_.push_back(start_number);
  operations_.emplace_back(" ");
}

void ArithmeticExpresion::AddOperations(const Operation &op) {
  numbers_.push_back(op.number);
  operations_.push_back(op.operation);
}

std::string ArithmeticExpresion::FullForm() const {
  std::string output = "(" + std::to_string(numbers_[0]) + ")";
  for (size_t i = 1; i < numbers_.size(); ++i) {
    if (i == numbers_.size() - 1) {
      std::string new_str = output + " " + operations_[i]
          + " " + std::to_string(numbers_[i]);
      output = new_str;
    }
    else {
      std::string new_str = "(" + output + " " + operations_[i]
          + " " + std::to_string(numbers_[i]) + ")";
      output = new_str;
    }
  }
  return output;
}

std::string ArithmeticExpresion::NormalForm() const {
  std::string output = std::to_string(numbers_[0]);
  for (size_t i = 1; i < numbers_.size(); ++i) {
    std::string new_str;
    if (GetPriority(operations_[i]) <= GetPriority(operations_[i - 1]) || i == 1) {
      new_str = output + " " + operations_[i] + " " + std::to_string(numbers_[i]);
    }
    else {
      new_str = "(" + output + ") " + operations_[i] + " " + std::to_string(numbers_[i]);
    }
    output = new_str;
  }
  return output;
}


int ArithmeticExpresion::GetPriority(const std::string &operation) const {
  if (operation == "+" || operation == "-") {
    return 0;
  }
  else if (operation == "*" || operation == "/") {
    return 1;
  }
  else {
    return -1;
  }
}