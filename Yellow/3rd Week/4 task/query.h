//
// Created by ilya on 08.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_3RD_WEEK_4_TASK_QUERY_H_
#define SECONDTWEEK_YELLOW_3RD_WEEK_4_TASK_QUERY_H_

#include <string>
#include <vector>
#include <iostream>

// Да, я знаю что так делать нельзя. Но это для экономии времени.
using namespace std;

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

istream& operator >> (istream& is, Query& q);

#endif //SECONDTWEEK_YELLOW_3RD_WEEK_4_TASK_QUERY_H_
