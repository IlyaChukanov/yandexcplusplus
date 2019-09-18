//
// Created by ilya on 15.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_FINAL_PROJECT_DATE_H_
#define SECONDTWEEK_YELLOW_FINAL_PROJECT_DATE_H_

#include <string>
#include <sstream>

class Date {
 public:
  Date();
  explicit Date(const std::string& input_date);

  int GetYear() const;
  int GetMonth() const;
  int GetDay() const;

  Date& operator=(const Date& rhs);

  std::string GetDate(char separator = '-') const;

 private:
  int year;
  int month;
  int day;

  // Проверка формата
  void CheckFormat(const std::string& input);
  bool CheckNumber(std::stringstream& ss, int& number);

  // Проверка корректности даты
  void CheckData();
};

bool operator<(const Date& lhs, const Date& rhs);

std::ostream& operator<(std::ostream& ostream, const Date& date);

#endif //SECONDTWEEK_YELLOW_FINAL_PROJECT_DATE_H_
