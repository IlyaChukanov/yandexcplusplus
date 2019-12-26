//
// Created by ilya on 29.09.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_2ND_WEEK_1_TASK_STUDENT_H_
#define YANDEXCPLUSPLUS_3_RED_2ND_WEEK_1_TASK_STUDENT_H_

#include <map>
#include <string>

using namespace std;

struct Student {
  string first_name;
  string last_name;
  map<string, double> marks;
  double rating;

  bool operator < (const Student& other) const {
    return GetName() < other.GetName();
  }

  bool Less(const Student& other) const {
    return rating > other.rating;
  }

  string GetName() const {
    return first_name + " " + last_name;
  }
};

#endif //YANDEXCPLUSPLUS_3_RED_2ND_WEEK_1_TASK_STUDENT_H_
