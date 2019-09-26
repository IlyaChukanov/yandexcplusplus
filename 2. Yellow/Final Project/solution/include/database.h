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
#include <queue>

#include "date.h"

class Database {
 public:

  void Print(std::ostream& out) const;
  void Add(const Date& date, const std::string& event);

  template <typename Predicate>
  std::vector<std::string> FindIf(Predicate predicate) const {
    std::vector<std::string> result;
    for (const auto& date : data_) {
      for (const auto& event : date.second.data_vector) {
        if (predicate(date.first, event)) {
          std::string bufer = date.first.GetDate() + " " + event;
          result.push_back(bufer);
        }
      }
    }
    return result;
  }

  template <typename Predicate>
  int RemoveIf(Predicate predicate) {
    int count = 0;
    for(auto date = data_.begin(); date != data_.end(); ) {
      // Удаление записи, если при данной дате условие TRUE в независимости от события
      if (predicate(date->first, "")) {
        count += date->second.data_vector.size();
        date = data_.erase(date);
      }
      else {
        auto new_predicate = [date, predicate](const std::string& event) { return !predicate(date->first, event);};
        auto start = std::stable_partition(date->second.data_vector.begin(), date->second.data_vector.end(), new_predicate);
        // Удаление всего массива, если он полностью соответствует условию
        if (start == date->second.data_vector.begin()) {
          count += date->second.data_vector.size();
          date = data_.erase(date);
        }
        else if (start != date->second.data_vector.end()) {
          for (auto curr = --date->second.data_vector.end(); curr != prev(start); curr--) {
            date->second.data_vector.pop_back();
            date->second.data_set.erase(*curr);
            count++;
          }
          if(date->second.data_vector.empty()) date = data_.erase(date);
          else ++date;
        }
        else {
          ++date;
        }
      }
    }
    return count;
  }

  std::string Last(const Date& date) const;

 private:

  struct Events {
    std::vector<std::string> data_vector;
    std::set<std::string> data_set;
  };

  std::map<Date, Events> data_;
};

#endif //SECONDTWEEK_YELLOW_FINAL_PROJECT_DATABASE_H_
