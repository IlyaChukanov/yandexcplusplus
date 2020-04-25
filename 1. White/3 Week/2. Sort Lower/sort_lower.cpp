//
// Created by ilya on 17.07.19.
//

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>



bool SortLower(const std::string& s1, const std::string& s2) {
  std::string lower_s1, lower_s2;
  lower_s1.reserve(s1.size());
  lower_s2.reserve(s2.size());
  for (const auto& c : s1) {
    lower_s1.push_back(tolower(c));
  }

  for (const auto& c : s2) {
    lower_s2.push_back(tolower(c));
  }

  return lower_s1 < lower_s2;
}

int main() {
  int n;
  std::cin >> n;
  std::vector<std::string> vec(n);
  for (auto& s : vec) {
    std::cin >> s;
  }

  std::sort(vec.begin(), vec.end(), SortLower);

  for (const auto& s : vec) {
    std::cout << s << " ";
  }
  return 0;
}
