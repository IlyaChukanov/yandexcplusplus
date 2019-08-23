//
// Created by ilya on 16.07.19.
//
// Хотел бы решить эту задачу через класс, но увы нельзя. Решу в лоб.

#include <vector>
#include <string>
#include <iostream>


int main() {

  // Количество запросов
  int q;
  std::cin >> q;
  // Количество спокойных
  int quiet_label = 0;

  // Количество беспокойных
  int worry = 0;
  int worry_label = 1;


  std::vector<int> query;
  for (int i = 0; i < q; ++i) {
    std::string request;
    std::cin >> request;

    if (request == "COME") {
      int k;
      std::cin >> k;
      if (k < 0) {
        for (int j = 0; j < abs(k); ++j) {
          if (query.back() == worry_label) worry--;
          query.pop_back();
        }
      }
      else {
        for (int j = 0; j < abs(k); ++j) {
          query.push_back(quiet_label);
        }
      }
    }

    if (request == "WORRY") {
      int curr;
      std::cin >> curr;
      worry++;
      query[curr] = worry_label;
    }

    if (request == "QUIET") {
      int curr;
      std::cin >> curr;
      worry--;
      query[curr] = quiet_label;
    }

    if (request == "WORRY_COUNT") {
      std::cout << worry << std::endl;
    }

  }

  return 0;
}