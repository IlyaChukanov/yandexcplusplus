//
// Created by ilya on 22.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_BUDGET_H_
#define YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_BUDGET_H_

#include <unordered_map>
#include <map>
#include <optional>
#include <vector>
#include <iostream>
#include "date.h"

class Budget {
 public:
  Budget() = default;

  double ComputeTotalIncome(const Date& from, const Date& to);

  void SetIncome(const Date& from, const Date& to, double value);

  void PayTax(const Date &from, const Date &to, double tax);

  void Spend(const Date& from, const Date& to, double value);

 private:
  static const int SECONDS_IN_DAY = 60 * 60 * 24;
  struct Day {
    double ComputeIncome() const {
      return income - expended;
    }
    void PayTax(double tax) {
      income *= 1 - tax/100.0;
    }
    double income;
    double expended;
  };
  std::unordered_map<time_t, Day> income_for_day_;
};

class BudgetManager {
public:
  BudgetManager(std::istream& stream) : stream_(stream) {}

  std::vector<double> Process();

private:
  BudgetManager() = delete;
  Budget budget_;
  std::istream& stream_;
};

void PrintResult(const std::vector<double>& result, std::ostream& stream);

#endif //YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_BUDGET_H_
