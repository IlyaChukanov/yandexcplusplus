//
// Created by ilya on 15.09.2019.
//

#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

#include "Figure.h"

int main() {
  std::vector<std::shared_ptr<Figure>> figures;
  for (std::string line; getline(std::cin, line); ) {
    std::istringstream is(line);

    std::string command;
    is >> command;
    if (command == "ADD") {
      figures.push_back(CreateFigure(is));
    } else if (command == "PRINT") {
      for (const auto& current_figure : figures) {
        std::cout << std::fixed << std::setprecision(3)
             << current_figure->Name() << " "
             << current_figure->Perimeter() << " "
             << current_figure->Area() << std::endl;
      }
    }
  }
  return 0;
}