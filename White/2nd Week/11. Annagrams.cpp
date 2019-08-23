//
// Created by ilya on 16.07.19.
//

#include <map>
#include <iostream>
#include <string>

std::map<char, int> BuildHist(const std::string &s) {
  std::map<char, int> res;
  for (const auto& c : s) {
    ++res[c];
  }
  return res;
}

bool IsPalindrom(const std::string &s1, const std::string &s2) {
  auto hist1 = BuildHist(s1);
  auto hist2 = BuildHist(s2);

  return hist1 == hist2;
}

int main() {
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::string s1, s2;
    std::cin >> s1 >> s2;
    auto res = IsPalindrom(s1, s2);
    if (res) std::cout << "YES" << std::endl;
    else std::cout << "NO" << std::endl;
  }
  return 0;
}