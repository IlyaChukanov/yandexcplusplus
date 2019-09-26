//
// Created by ilya on 22.08.19.
//
#include <string>
#include <vector>
#include <iostream>

struct Student {
  std::string first_name;
  std::string second_name;
  int day;
  int month;
  int year;
};

int main() {
  int N;
  std::cin >> N;
  std::vector<Student> students(N);
  for (auto& s : students) {
    std::string f_name, s_name;
    int day, month, year;
    std::cin >> f_name >> s_name;
    std::cin >> day >> month >> year;
    s = {f_name, s_name, day, month, year};
  }
  int M;
  std::cin >> M;
  for (int i = 0; i < M; ++i) {
    std::string command;
    int index;
    std::cin >> command >> index;
    index--;
    if (command == "name" && index >= 0 && index < N) {
      std::cout << students[index].first_name << " " << students[index].second_name << std::endl;
    }
    else if (command == "date" && index >= 0 && index < N) {
      std::cout << students[index].day << "." << students[index].month << "." << students[index].year << std::endl;
    }
    else {
      std::cout << "bad request" << std::endl;
    }
  }
  return 0;
}