//
// Created by ilya on 15.09.2019.
//

#include <string>
#include <sstream>
#include <tuple>
#include <iomanip>

#include "../Include/date.h"

Date::Date() : year(0), month(0), day(0) {}

Date::Date(const std::string &input_date) {
  CheckFormat(input_date);
  CheckData();
}

int Date::GetYear() const { return year; }

int Date::GetMonth() const { return month; }

int Date::GetDay() const { return day; }

Date& Date::operator=(const Date& rhs) {
  year = rhs.year;
  month = rhs.month;
  day = rhs.day;
  return *this;
}

std::string Date::GetDate(char separator) const {
  std::stringstream so;
  so << std::setfill('0');
  if (year < 0) {
    so << '-';
  }
  so << std::setw(4) << abs(year);
  so << separator;
  so << std::setw(2) << month;
  so << separator;
  so << std::setw(2) << day;
  return so.str();
}


void Date::CheckFormat(const std::string &input) {
  std::stringstream ss(input);
  int count_data = 3;
  int date_data[3] = {-1, -1, -1};
  year = date_data[0];
  month = date_data[1];
  day = date_data[2];

  if (!CheckNumber(ss, date_data[0])) {
    std::stringstream so;
    so << "Wrong date format: " << input;
    throw std::runtime_error(so.str());
  }

  char input_date_separator = '-';
  for (int i = 1; i < count_data; ++i) {
    if (ss.peek() != input_date_separator) {
      std::stringstream so;
      so << "Wrong date format: " << input;
      throw std::runtime_error(so.str());
    }
    else {
      ss.ignore(1);
      if (!CheckNumber(ss, date_data[i])) {
        std::stringstream so;
        so << "Wrong date format: " << input;
        throw std::runtime_error(so.str());
      }
    }
  }

  if (ss.rdbuf()->in_avail() != 0) {
    std::stringstream so;
    so << "Wrong date format: " << input;
    throw std::runtime_error(so.str());
  }
  year = date_data[0];
  month = date_data[1];
  day = date_data[2];
}

bool Date::CheckNumber(std::stringstream& ss, int& number) {
  char sign = '+';
  if (ss.peek() == '+' || ss.peek() == '-') {
    sign = ss.get();
  }
  if (ss.peek() >= '0' && ss.peek() <= '9') {
    ss >> number;
    if (sign == '-') {
      number = -number;
    }
  }
  else {
    return false;
  }
  return true;
}

void Date::CheckData() {
  if (month < 1 || month > 12) {
    std::stringstream ss;
    ss << "Month value is invalid: " << month;
    throw std::runtime_error(ss.str());
  }

  if (day < 1 || day > 31) {
    std::stringstream ss;
    ss << "Day value is invalid: " << day;
    throw std::runtime_error(ss.str());
  }
}

bool operator<(const Date& lhs, const Date& rhs) {
  int l_year = lhs.GetYear(), l_month = lhs.GetMonth(), l_day = lhs.GetDay();
  int r_year = rhs.GetYear(), r_month = rhs.GetMonth(), r_day = rhs.GetDay();
  return (std::tie(l_year, l_month, l_day) < std::tie(r_year, r_month, r_day));
}
