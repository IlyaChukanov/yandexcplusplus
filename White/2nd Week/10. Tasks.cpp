//
// Created by ilya on 16.07.19.
//

#include <vector>
#include <string>
#include <iostream>

int main() {
  int current_month = 0;
  std::vector<int> day_in_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  std::vector<std::vector<std::string>> tasks(day_in_month[current_month]);

  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::string request;
    std::cin >> request;

    if (request == "ADD") {
      int j;
      std::string doing;
      std::cin >> j >> doing;
      j--;
      tasks[j].push_back(doing);
    }

    if (request == "DUMP") {
      int j;
      std::cin >> j;
      j--;
      if (tasks[j].empty()) {
        std::cout << 0 << std::endl;
      }
      else {
        std::cout << tasks[j].size() << " ";
        for (auto d : tasks[j]) {
          std::cout << d << " ";
        }
        std::cout << std::endl;
      }
    }

    if (request == "NEXT") {
      int curr_days = day_in_month[current_month];
      current_month = (current_month + 1) % 12;
      int new_days = day_in_month[current_month];

      if (new_days < curr_days) {
        for (int j = new_days; j < curr_days; ++j) {
          tasks[new_days - 1].insert(tasks[new_days - 1].end(), tasks[j].begin(), tasks[j].end());
        }
      }
      tasks.resize(new_days);
    }


  }
  return 0;
}