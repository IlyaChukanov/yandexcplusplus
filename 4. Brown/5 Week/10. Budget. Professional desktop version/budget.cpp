//
// Created by ilya on 22.11.2019.
//

#include "budget.h"

double Budget::ComputeTotalIncome(const Date &from, const Date &to) {
  auto time_from = from.AsTimestamp();
  auto time_to = to.AsTimestamp();

  double res = 0;
  for (auto curr_day = time_from; curr_day <= time_to; curr_day += SECONDS_IN_DAY) {
    res += income_for_day_[curr_day].ComputeIncome();
  }
  return res;
}

void Budget::PayTax(const Date &from, const Date &to, double tax) {
  auto time_from = from.AsTimestamp();
  auto time_to = to.AsTimestamp();
  for (auto curr_day = time_from; curr_day <= time_to; curr_day += SECONDS_IN_DAY) {
    income_for_day_[curr_day].PayTax(tax);
  }
}

void Budget::SetIncome(const Date &from, const Date &to, double value) {
  auto time_from = from.AsTimestamp();
  auto time_to = to.AsTimestamp();
  auto diff = Date::ComputeDaysDiff(to, from) + 1;
  double value_per_day = value / diff;
  for (auto curr_day = time_from; curr_day <= time_to; curr_day += SECONDS_IN_DAY) {
    income_for_day_[curr_day].income += value_per_day;
  }
}

void Budget::Spend(const Date &from, const Date &to, double value) {
  auto time_from = from.AsTimestamp();
  auto time_to = to.AsTimestamp();
  auto diff = Date::ComputeDaysDiff(to, from) + 1;
  double value_per_day = value / diff;
  for (auto curr_day = time_from; curr_day <= time_to; curr_day += SECONDS_IN_DAY) {
    income_for_day_[curr_day].expended += value_per_day;
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
      double value;
      stream_ >> from >> to >> value;
      budget_.PayTax(Date(from), Date(to), value);
    }
    if (command == "ComputeIncome") {
      std::string from, to;
      stream_ >> from >> to;
      result.push_back(budget_.ComputeTotalIncome(Date(from), Date(to)));
    }
    if (command == "Spend") {
      std::string from, to;
      double value = 0;
      stream_ >> from >> to >> value;
      budget_.Spend(Date(from), Date(to), value);
    }
  }
  return result;
}

void PrintResult(const std::vector<double>& result, std::ostream& stream) {
  for (const auto& elem : result) {
    stream << elem << std::endl;
  }
}