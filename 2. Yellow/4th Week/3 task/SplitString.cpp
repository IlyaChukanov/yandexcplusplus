//
// Created by ilya on 08.09.2019.
//

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>


std::vector<std::string> SplitIntoWords(const std::string& s) {
  std::vector<std::string> result;
  auto curr = s.begin();
  while (curr != s.end()) {
    auto space = std::find(curr, s.end(), ' ');
    result.emplace_back(std::string(curr, space));
    if (space != s.end()) curr = (space + 1);
    else curr = s.end();
  }
  return result;
}

int main() {
  std::string s = "C Cpp Java Python ABXC";

  std::vector<std::string> words = SplitIntoWords(s);
  std::cout << words.size() << " ";
  for (auto it = begin(words); it != end(words); ++it) {
    if (it != begin(words)) {
      std::cout << "/";
    }
    std::cout << *it;
  }
  std::cout << std::endl;

  return 0;
}