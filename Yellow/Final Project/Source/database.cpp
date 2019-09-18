//
// Created by ilya on 15.09.2019.
//

#include <iostream>
#include <algorithm>

#include "../Include/database.h"

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

template <typename Predicate>
std::vector<std::string> Database::FindIf(Predicate predicate) const {
  std::vector<std::string> result;
  for (const auto& date : data_) {
    for (const auto& event : date.second) {
      if (predicate(date.first, event)) {
        std::string bufer = date.first.GetDate() + " " + event;
        result.push_back(bufer);
      }
    }
  }
}

template <typename Predicate>
int Database::RemoveIf(Predicate predicate) {
  int count = 0;
  for (auto date : data_) {
    auto new_predicate = [date, predicate](const std::string& event) { return predicate(date, event);};
    date.second.erase(std::remove_if(date.second.begin(),
                                     date.second.end(),
                                     new_predicate),
                      date.second.end());
    if (data_.at(date.first).empty()) {
      data_.erase(date.first);
    }
  }
}