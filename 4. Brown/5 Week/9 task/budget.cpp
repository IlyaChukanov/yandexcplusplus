//
// Created by ilya on 22.11.2019.
//

#include "budget.h"


double Budget::ComputeTotalIncome(const Date &from, const Date &to) {

}

void Budget::PayTax(const Date &from, const Date &to) {

}

void Budget::SetIncome(const Date &from, const Date &to, double value) {
  auto start = income_for_date_.insert({from, value});
  auto end = income_for_date_.insert({to, std::nullopt});

  /// Если вставка прошла неудачно
  if (!start.second) {
    if (start.first != income_for_date_.end()) {
      start.first->second = start.first->second.value() + value;
    }
  }

  /// Если вставка прошла успешно
  // Если дата начала находится внутри другого интервала
  // то добавляем к началу интервала значение другого интервала
  if (start.first != income_for_date_.begin()) {
    auto prev = std::prev(start.first);
    if (prev->second) {
      start.first->second = value + prev->second.value();
    }
  }
  // Обновляем все валютные интервалы внутри нашего
  for (auto curr = std::next(start.first); curr != end.first; curr = std::next(curr)) {
    if (curr->second) {
      curr->second = curr->second.value() + value;
    }
    else {
      curr->second = value;
    }
  }
}
