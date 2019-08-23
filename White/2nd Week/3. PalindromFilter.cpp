//
// Created by ilya on 16.07.19.
//

#include <iostream>
#include <string>
#include <vector>

bool IsPalindrom(std::string s) {
  for (size_t i = 0; i < s.size() / 2; ++i) {
    if (s[i] != s[s.size() - i - 1]) {
      return false;
    }
  }
  return true;
}

std::vector<std::string> PalindromFilter(std::vector<std::string> &strings, int min_length) {

  std::vector<std::string> res;
  for (auto s : strings) {
    if (IsPalindrom(s) && s.size() >= min_length) res.push_back(s);
  }
  return res;
}

/*
int main() {
  int n;
  std::cin >> n;
  std::vector<std::string> input, output;
  for (int i = 0; i < n; ++i) {
    std::string buff;
    std::cin >> buff;
    input.push_back(buff);
  }
  output = PalindromFilter(input);
  for (auto s : output) {
    std::cout << s << std::endl;
  }
  return 0;
}
 */