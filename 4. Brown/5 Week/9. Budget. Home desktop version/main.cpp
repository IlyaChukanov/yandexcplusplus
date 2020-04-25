//
// Created by ilya on 22.11.2019.
//
#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include "budget.h"

#include <iostream>
#include <iomanip>

int main() {
  //TestAll();
  BudgetManager b(std::cin);
  auto res = b.Process();
  std::cout.precision(25);
  for (const auto& elem : res) {
    std::cout << elem << std::endl;
  }
  return 0;
}