//
// Created by ilya on 22.11.2019.
//

#include "budget.h"

double Budget::ComputeTotalIncome(const Date &from, const Date &to) {
  auto time_from = from.AsTimestamp();
  auto time_to = to.AsTimestamp();

  double res = 0;
  for (auto curr_day = time_from; curr_day <= time_to; curr_day += SECONDS_IN_DAY) {
    res += income_for_day_[curr_day];
  }
  return res;
}

void Budget::PayTax(const Date &from, const Date &to) {
  auto time_from = from.AsTimestamp();
  auto time_to = to.AsTimestamp();
  for (auto curr_day = time_from; curr_day <= time_to; curr_day += SECONDS_IN_DAY) {
    income_for_day_[curr_day] *= 0.87;
  }
}

void Budget::SetIncome(const Date &from, const Date &to, double value) {
  auto time_from = from.AsTimestamp();
  auto time_to = to.AsTimestamp();
  auto diff = Date::ComputeDaysDiff(to, from) + 1;
  double value_per_day = value / diff;
  for (auto curr_day = time_from; curr_day <= time_to; curr_day += SECONDS_IN_DAY) {
    income_for_day_[curr_day] += value_per_day;
  }
}

std::vector<double> BudgetManager::Process() {
  std::vector<double> result;
  size_t count = 0;
  stream_ >> count;
  for (size_t i = 0; i < count; ++i) {
    std::string command;
    stream_ >> command;
    if (command == "Earn") {
      std::string from, to;
      double value = 0;
      stream_ >> from >> to >> value;
      budget_.SetIncome(Date(from), Date(to), value);
    }
    if (command == "PayTax") {
      std::string from, to;
      stream_ >> from >> to;
      budget_.PayTax(Date(from), Date(to));
    }
    if (command == "ComputeIncome") {
      std::string from, to;
      stream_ >> from >> to;
      result.push_back(budget_.ComputeTotalIncome(Date(from), Date(to)));
    }
  }
  return result;
}