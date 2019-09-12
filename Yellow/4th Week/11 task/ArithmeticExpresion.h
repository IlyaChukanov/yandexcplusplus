//
// Created by ilya on 12.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_4TH_WEEK_11_TASK_ARITHMETICEXPRESION_H_
#define SECONDTWEEK_YELLOW_4TH_WEEK_11_TASK_ARITHMETICEXPRESION_H_

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

#endif //SECONDTWEEK_YELLOW_4TH_WEEK_11_TASK_ARITHMETICEXPRESION_H_
