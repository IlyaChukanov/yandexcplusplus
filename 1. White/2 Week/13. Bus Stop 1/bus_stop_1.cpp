//
// Created by ilya on 16.07.19.
//


#include <map>
#include <string>
#include <iostream>
#include <vector>

int main() {

  std::map<std::string, std::vector<std::string>> buses_for_stop;
  std::map<std::string, std::vector<std::string>> stops_for_bus;

  int q;
  std::cin >> q;
  for (int i = 0; i < q; ++i) {
    std::string request;
    std::cin >> request;
    if (request == "NEW_BUS") {
      std::string bus;
      std::cin >> bus;

      int count;
      std::cin >> count;

      std::vector<std::string> stops(count);
      for (auto & s : stops) {
        std::cin >> s;
        buses_for_stop[s].push_back(bus);
      }
      stops_for_bus[bus] = stops;
    }

    if (request == "BUSES_FOR_STOP") {
      std::string stop;
      std::cin >> stop;
      if (!buses_for_stop.count(stop)) std::cout << "No stop" << std::endl;
      else {
        for (auto b : buses_for_stop[stop]) {
          std::cout << b << " ";
        }
        std::cout << std::endl;
      }
    }

    if (request == "STOPS_FOR_BUS") {
      std::string bus;
      std::cin >> bus;
      if (!stops_for_bus.count(bus)) {
        std::cout << "No bus" << std::endl;
      }
      else {
        for (auto stop : stops_for_bus[bus]) {
          std::cout << "Stop " << stop << ": ";
          int count = 0;
          for (auto b : buses_for_stop[stop]) {
            if (b == bus) continue;
            else {
              std::cout << b << " ";
              count++;
            }
          }
          if (!count) {
            std::cout << "no interchange";
          }
          std::cout << std::endl;
        }
      }
    }

    if (request == "ALL_BUSES") {
      if (stops_for_bus.empty()) {
        std::cout << "No buses" << std::endl;
      }
      else {
        for (auto b : stops_for_bus) {
          std::cout << "Bus " << b.first << ": ";
          for (auto s : stops_for_bus[b.first]) {
            std::cout << s << " ";
          }
          std::cout << std::endl;
        }
      }
    }

  }
  return 0;
}