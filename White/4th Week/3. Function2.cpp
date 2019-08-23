//
// Created by ilya on 21.08.19.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class FunctionPart {
public:
  FunctionPart(char new_operator, double new_value) {
    operator_ = new_operator;
    value_ = new_value;
  }

  double Apply(double curr_value) const {
    switch (operator_) {
      case '+': {
        return curr_value + value_;
      }
      case '-': {
        return curr_value - value_;
      }
      case '*': {
        return curr_value * value_;
      }
      case '/': {
        return curr_value / value_;
      }
      default: {
        return curr_value;
      }
    }
  }

  void Invert() {
    switch (operator_) {
      case '+': {
        operator_ = '-';
        break;
      }
      case '-': {
        operator_ = '+';
        break;
      }
      case '*': {
        operator_ = '/';
        break;
      }
      case '/': {
        operator_ = '*';
        break;
      }
      default: {
        break;
      }
    }
  }
private:
  char operator_;
  double value_;
};

class Function {
public:
  void AddPart(char curr_operator, double value) {
    parts.push_back({curr_operator, value});
  }

  double Apply(double curr_value) const {
    for (const auto& item : parts) {
      curr_value = item.Apply(curr_value);
    }
    return curr_value;
  }

  void Invert() {
    for (auto& item : parts) {
      item.Invert();
    }
    std::reverse(parts.begin(), parts.end());
  }

private:
  std::vector<FunctionPart> parts;
};

