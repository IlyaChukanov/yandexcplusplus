//
// Created by ilya on 08.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_3RD_WEEK_4_TASK_RESPONSES_H_
#define SECONDTWEEK_YELLOW_3RD_WEEK_4_TASK_RESPONSES_H_

#include <iostream>
#include <sstream>
#include <vector>

// Да, я знаю что так делать нельзя. Но это для экономии времени.
using namespace std;


template <typename Container>
ostream& ContainerPrint(ostream& os, const Container& con, char sep = ' ') {
  for (const auto& item : con) {
    os << item << sep;
  }
}

struct BusesForStopResponse {
  std::vector<std::string> response;
  stringstream stream_response;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r);

struct StopsForBusResponse {
  std::vector<std::string> response;
  stringstream stream_response;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r);

struct AllBusesResponse {
  std::vector<std::string> response;
  stringstream stream_response;
};

ostream& operator << (ostream& os, const AllBusesResponse& r);

#endif //SECONDTWEEK_YELLOW_3RD_WEEK_4_TASK_RESPONSES_H_
