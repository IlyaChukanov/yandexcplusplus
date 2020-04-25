//
// Created by ilya on 22.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_BUDGET_H_
#define YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_BUDGET_H_

#include <unordered_map>
#include <map>
#include <optional>
#include <vector>

#include "date.h"

class Budget {
 public:
  Budget() = default;

  double ComputeTotalIncome(const Date& from, const Date& to);

  void SetIncome(const Date& from, const Date& to, double value);

  void PayTax(const Date& from, const Date& to);

 private:
  static const int SECONDS_IN_DAY = 60 * 60 * 24;
  std::unordered_map<time_t, double> income_for_day_;
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
#endif //YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_BUDGET_H_
