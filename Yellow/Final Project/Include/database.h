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

#include "date.h"

class Database {
 public:

  void Print(std::ostream& out) const;

  std::string Last(const Date& date) const;

  void Add(const Date& date, const std::string& event);

  template <typename Predicate>
  int RemoveIf(Predicate predicate);

  template <typename Predicate>
  std::vector<std::string> FindIf(Predicate predicate);

 private:
  std::map<Date, std::vector<std::string>> data_;
};

#endif //SECONDTWEEK_YELLOW_FINAL_PROJECT_DATABASE_H_
