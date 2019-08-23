//
// Created by ilya on 22.08.19.
//

#include <string>
#include <fstream>
#include <iostream>

int main() {
  std::string my_path = "../4th Week/input.txt";
  std::string path = "input.txt";
  std::ifstream input(path);
  if (input.is_open()) {
    std::string line;
    while (getline(input, line)) {
      std::cout << line << std::endl;
    }
  }
  return 0;
}

