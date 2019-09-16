//
// Created by ilya on 15.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_FINAL_PROJECT_DATABASE_H_
#define SECONDTWEEK_YELLOW_FINAL_PROJECT_DATABASE_H_

#include <set>
#include <map>
#include <string>
#include <iostream>

#include "date.h"

Date ParseDate(std::istream& input) {

}

std::string ParseEvent(std::istream& is) {

}

class Database {
 public:

  void Print(std::ostream& out);

 private:
  std::map<Date, std::set<std::string>> data_;
};

#endif //SECONDTWEEK_YELLOW_FINAL_PROJECT_DATABASE_H_
