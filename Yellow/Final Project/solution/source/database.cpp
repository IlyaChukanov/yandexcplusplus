//
// Created by ilya on 15.09.2019.
//

#include <iostream>
#include <algorithm>

#include "database.h"

void Database::Print(std::ostream &out) const {
  for (const auto& date : data_) {
    for (const auto& event : date.second) {
      out << date.first.GetDate() << " " << event << std::endl;
    }
  }
}

std::string Database::Last(const Date &date) const {
  auto vector = data_.upper_bound(date);
  if (vector == data_.end()) {
    return "No print";
  }
  else {
    return vector->second.back();
  }
}

void Database::Add(const Date &date, const std::string &event) {
  if (data_.count(date)) {
    auto container = data_.at(date);
    auto repeat = std::find(container.begin(), container.end(), event);
    if (repeat == container.end()) {
      data_.at(date).push_back(event);
    }
  }
  else {
    data_[date].push_back(event);
  }
}

