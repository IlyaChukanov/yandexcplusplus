//
// Created by ilya on 17.07.19.
//

#include <map>
#include <vector>
#include <string>
#include <iostream>

int main() {

  int free_bus_number = 1;
  std::map<std::vector<std::string>, int> buses;

  int q;
  std::cin >> q;

  for (int i = 0; i < q; ++i) {
    int count;
    std::cin >> count;
    std::vector<std::string> stops(count);
    for (auto &s : stops) {
      std::cin >> s;
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