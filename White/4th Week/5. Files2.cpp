//
// Created by ilya on 22.08.19.
//

#include <string>
#include <fstream>
#include <iostream>

int main() {
  std::string my_in_path = "../4th Week/input.txt", my_out_path = "../4th Week/output.txt";
  std::string in_path = "input.txt", out_path = "output.txt";
  std::ifstream input(in_path);
  std::ofstream output(out_path);
  if (input.is_open()) {
    std::string line;
    while (getline(input, line)) {
      output << line << std::endl;
    }
  }
  return 0;
}

