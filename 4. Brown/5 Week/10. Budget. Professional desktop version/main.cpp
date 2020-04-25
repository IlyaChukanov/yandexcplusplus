//
// Created by ilya on 22.11.2019.
//
#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include "budget.h"

#include <iomanip>

int main() {
  std::cout.precision(25);
  PrintResult(BudgetManager(std::cin).Process(), std::cout);
  return 0;
}