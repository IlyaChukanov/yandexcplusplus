//
// Created by ilya on 07.09.2019.
//

//#include "2. Phone Number.h"
#include "phone_number.h"
#include <sstream>
#include <string>

PhoneNumber::PhoneNumber(const string &international_number) {

  // +XXX-YYY-ZZZZZZ
  std::stringstream ss(international_number);
  if (ss.peek() != '+') {
    throw std::invalid_argument("First character is not '+'");
  }
  // Skip '+'
  ss.ignore();
  std::string country_code, city_code, local_number;

  // Read from '+' to first '-' == XXX
  while(ss.peek() != '-' && ss.good()) {
    country_code.push_back(ss.get());
  }
  // Skip first '-'
  ss.ignore();
  // Read from first '-' to second '-' == YYY
  while(ss.peek() != '-' && ss.good()) {
    city_code.push_back(ss.get());
  }
  // Skip second '-'
  ss.ignore();
  // Read local number == ZZZZZZZ
  ss >> local_number;
  if (country_code.empty() ||
      city_code.empty() ||
      local_number.empty()) {
    throw std::invalid_argument("Not enough codes");
  }
  country_code_ = country_code;
  city_code_ = city_code;
  local_number_ = local_number;
}

string PhoneNumber::GetCountryCode() const {
  return country_code_;
}

string PhoneNumber::GetCityCode() const {
  return city_code_;
}

string PhoneNumber::GetLocalNumber() const {
  return local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
  return "+" + country_code_ + "-" + city_code_ + "-" + local_number_;
}