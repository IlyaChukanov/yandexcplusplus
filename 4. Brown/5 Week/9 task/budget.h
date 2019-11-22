//
// Created by ilya on 22.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_BUDGET_H_
#define YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_BUDGET_H_

#include <map>
#include <optional>

#include "data.h"

class Budget {
 public:
  Budget() = default;

  double ComputeTotalIncome(const Date& from, const Date& to);

  void SetIncome(const Date& from, const Date& to, double value);

  void PayTax(const Date& from, const Date& to);

 private:
  std::map<Date, std::optional<double>> income_for_date_;
};

#endif //YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_BUDGET_H_
