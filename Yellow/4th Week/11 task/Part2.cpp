//
// Created by ilya on 12.09.2019.
//

#include <string>
#include <iostream>
#include <vector>

struct Operation {
  std::string operation;
  int number;
};

std::istream& operator>>(std::istream& stream, Operation& op);

class ArithmeticExpresion {

 public:

  explicit ArithmeticExpresion(int start_number);

  void AddOperations(const Operation& op);

  std::string FullForm() const;
  std::string NormalForm() const;

 private:

  int GetPriority(const std::string& operation) const;

  std::vector<int> numbers_;
  std::vector<std::string> operations_;

};

std::istream& operator>>(std::istream& stream, Operation& op) {
  stream >> op.operation >> op.number;
  return stream;
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
  std::string output = std::to_string(numbers_[0]);
  for (size_t i = 1; i < numbers_.size(); ++i) {
    std::string new_str = "(" + output + ") " + operations_[i] + " " + std::to_string(numbers_[i]);
    output = new_str;
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

int main() {
  int basic = 0;
  std::cin >> basic;
  ArithmeticExpresion ae(basic);

  int n = 0;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    Operation op;
    std::cin >> op;
    ae.AddOperations(op);
  }
  std::cout << ae.NormalForm() << std::endl;
}