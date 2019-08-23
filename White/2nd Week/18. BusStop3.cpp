//
// Created by ilya on 17.07.19.
//

#include <iostream>
#include <string>
#include <set>
#include <map>

int main() {
  int free_bus_number = 1;
  std::map<std::set<std::string>, int> buses;

  int q;
  std::cin >> q;

  for (int i = 0; i < q; ++i) {
    int count;
    std::cin >> count;
    std::set<std::string> stops;
    for (int j = 0; j < count; ++j) {
      std::string s;
      std::cin >> s;
      stops.insert(s);
    }

    if (!buses.count(stops)) {
      buses[stops] = free_bus_number;
      std::cout << "New bus " << free_bus_number << std::endl;
      free_bus_number++;
    }
    else {
      std::cout << "Already exists for " << buses[stops] << std::endl;
    }
  }

  return 0;
}