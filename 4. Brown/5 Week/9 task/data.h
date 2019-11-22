//
// Created by ilya on 22.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_DATA_H_
#define YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_DATA_H_
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

  time_t AsTimestamp() const;

  static int ComputeDaysDiff(const Date& date_to, const Date& date_from);

 private:
  int year_;
  int month_;
  int day_;

  // Проверка формата
  void CheckFormat(const std::string& input);
  bool CheckNumber(std::stringstream& ss, int& number);
  // Проверка корректности даты
  void CheckData();
};

bool operator<(const Date& lhs, const Date& rhs);

bool operator==(const Date& lhs, const Date& rhs);

#endif //YANDEXCPLUSPLUS_4_BROWN_5_WEEK_9_TASK_DATA_H_
