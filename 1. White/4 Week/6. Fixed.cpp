//
// Created by ilya on 22.08.19.
//

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

int main() {
  std::string my_path = "../4 Week/input.txt";
  std::string path = "input.txt";
  std::ifstream input(path);
  std::cout << std::fixed << std::setprecision(3);

  double a;
  while (input >> a) {
    std::cout << a << std::endl;
  }


  return 0;
}

