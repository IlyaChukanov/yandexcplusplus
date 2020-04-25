//
// Created by ilya on 17.07.19.
//

#include <string>
#include <set>
#include <map>

std::set<std::string> BuildMapValuesSet(const std::map<int, std::string> &m) {
  std::set<std::string> set;
  for (const auto& item : m) {
    set.insert(item.second);
  }
  return set;
}