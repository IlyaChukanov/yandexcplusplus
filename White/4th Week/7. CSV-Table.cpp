//
// Created by ilya on 22.08.19.
//

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

int main() {
  int N, M;
  int width = 10;
  std::string my_path = "../4th Week/input.txt";
  std::string path = "input.txt";

  std::ifstream input(path);
  if (input.is_open()) {
    input >> N;
    input.ignore(1);
    input >> M;
    std::cout << std::setfill(' ');
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < M; ++j) {
        int bufer;
        input >> bufer;
        std::cout << std::setw(width) << bufer;
        if (j != M - 1) {
          input.ignore(1);
          std::cout << ' ';
        }
      }
      if (i != N - 1) std::cout << std::endl;
    }
  }
  return 0;
}