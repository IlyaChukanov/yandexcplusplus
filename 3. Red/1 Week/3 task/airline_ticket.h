//
// Created by ilya on 27.09.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_1TH_WEEK_3_TASK_AIRLINE_TICKET_H_
#define YANDEXCPLUSPLUS_3_RED_1TH_WEEK_3_TASK_AIRLINE_TICKET_H_

#include <string>
#include <tuple>

struct Date {
  int32_t year, month, day;
};

struct Time {
  int32_t hours, minutes;
};

bool operator<(const Date& lhs, const Date& rhs);
bool operator<(const Time& lhs, const Time& rhs);
bool operator==(const Date& lhs, const Date& rhs);
bool operator==(const Time& lhs, const Time& rhs);
std::ostream& operator<<(std::ostream& out, const Date& rhs);
std::ostream& operator<<(std::ostream& out, const Time& rhs);

struct AirlineTicket {
  std::string from;
  std::string to;
  std::string airline;
  Date departure_date;
  Time departure_time;
  Date arrival_date;
  Time arrival_time;
  int32_t price;
};

#endif //YANDEXCPLUSPLUS_3_RED_1TH_WEEK_3_TASK_AIRLINE_TICKET_H_
