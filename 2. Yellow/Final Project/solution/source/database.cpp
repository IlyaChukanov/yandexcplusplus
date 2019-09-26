//
// Created by ilya on 15.09.2019.
//

#include <iostream>
#include <algorithm>

#include "database.h"

void Database::Print(std::ostream &out) const {
  for (const auto& date : data_) {
    for (const auto& event : date.second.data_vector) {
      out << date.first.GetDate() << " " << event << std::endl;
    }
  }
}

std::string Database::Last(const Date &date) const {
  auto last = data_.upper_bound(date);
  if (last == data_.begin()) {
    return "No entries";
  }
  else {
    last = prev(last);
    return last->first.GetDate() + " " + last->second.data_vector.back();
  }
}

void Database::Add(const Date &date, const std::string &event) {
  auto result = data_[date].data_set.insert(event);
  if (result.second) {
    data_[date].data_vector.push_back(event);
  }
}