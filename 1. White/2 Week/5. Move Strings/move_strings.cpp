//
// Created by ilya on 16.07.19.
//

#include <vector>
#include <string>

void MoveStrings(std::vector<std::string> &source, std::vector<std::string> &destination) {

  for (auto s : source) {
    destination.push_back(s);
  }
  source.clear();
}