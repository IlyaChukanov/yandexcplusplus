//
// Created by ilya on 15.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_FINAL_PROJECT_DATABASE_H_
#define SECONDTWEEK_YELLOW_FINAL_PROJECT_DATABASE_H_

#include <set>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "date.h"

class Database {
 public:

  void Print(std::ostream& out) const;

  std::string Last(const Date& date) const;

  void Add(const Date& date, const std::string& event);

  template <typename Predicate>
  std::vector<std::string> FindIf(Predicate predicate) const {
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
  int RemoveIf(Predicate predicate) {
    int count = 0;
    for (auto date : data_) {
      auto new_predicate = [date, predicate](const std::string& event) { return predicate(date.first, event);};
      date.second.erase(std::remove_if(date.second.begin(),
                                       date.second.end(),
                                       new_predicate),
                        date.second.end());
      if (data_.at(date.first).empty()) {
        data_.erase(date.first);
      }
    }
  }

 private:
  std::map<Date, std::vector<std::string>> data_;
};

#endif //SECONDTWEEK_YELLOW_FINAL_PROJECT_DATABASE_H_
