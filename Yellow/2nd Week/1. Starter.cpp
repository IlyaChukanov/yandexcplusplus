#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <sstream>

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

istream& operator >> (istream& is, Query& q) {
  std::string operation_code;
  is >> operation_code;
  if (operation_code == "NEW_BUS") {
    q.type = QueryType::NewBus;
    is >> q.bus;
    int stop_count;
    is >> stop_count;
    std::vector<string> stops(stop_count);
    stops.resize(stop_count);
    for (auto& stop : stops) {
      is >> stop;
      stops.push_back(stop);
    }
    q.stops = stops;
  }
  else if (operation_code == "BUSES_FOR_STOP") {
    q.type = QueryType::BusesForStop;
    is >> q.stop;
  }
  else if (operation_code == "STOPS_FOR_BUS") {
    q.type = QueryType::StopsForBus;
    is >> q.bus;
  }
  else if (operation_code == "ALL_BUSES") {
    q.type = QueryType::AllBuses;
  }

  return is;
}

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

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
  //ContainerPrint(os, r.response);
  os << r.stream_response.str();
  return os;
}

struct StopsForBusResponse {
  std::vector<std::string> response;
  stringstream stream_response;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
  //ContainerPrint(os, r.response);
  os << r.stream_response.str();
  return os;
}

struct AllBusesResponse {
  std::vector<std::string> response;
  stringstream stream_response;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
  //ContainerPrint(os, r.response);
  os << r.stream_response.str();
  return os;
}

class BusManager {
public:
  void AddBus(const string& bus, const vector<string>& stops) {
    for (auto& stop : stops) {
      stops_to_buses[stop].push_back(bus);
    }
  }

  BusesForStopResponse GetBusesForStop(const string& stop) const {
    BusesForStopResponse response;
    if (stops_to_buses.count(stop) == 0) {
      response.stream_response << "No stop" << endl;
    }
    else {
      if (stops_to_buses.count(stop)) {
        for (const string& bus : stops_to_buses.at(stop)) {
          response.stream_response << bus << " ";
        }
        response.stream_response << endl;
      }
    }
    return response;
  }

  StopsForBusResponse GetStopsForBus(const string& bus) const {
    StopsForBusResponse response;
    if (buses_to_stops.count(bus) == 0) {
      response.stream_response << "No bus" << endl;
    }
    else {
      if (buses_to_stops.count(bus)) {
        for (const string& stop : buses_to_stops.at(bus)) {
          response.stream_response << "Stop " << stop << ": ";

          if (!stops_to_buses.count(stop)) continue;
          if (stops_to_buses.at(stop).size() == 1) {
            response.stream_response << "no interchange";
          } else {
            for (const string& other_bus : stops_to_buses.at(stop)) {
              if (bus != other_bus) {
                response.stream_response << other_bus << " ";
              }
            }
          }
          response.stream_response << endl;
        }
      }
    }
    return response;
  }

  AllBusesResponse GetAllBuses() const {
    AllBusesResponse response;
    if (buses_to_stops.empty()) {
      response.stream_response << "No buses" << endl;
    } else {
      for (const auto& bus_item : buses_to_stops) {
        response.stream_response << "Bus " << bus_item.first << ": ";
        for (const string& stop : bus_item.second) {
          response.stream_response << stop << " ";
        }
        response.stream_response << endl;
      }
    }
    return response;
  }
 private:
  map<string, vector<string>> buses_to_stops, stops_to_buses;
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
}
