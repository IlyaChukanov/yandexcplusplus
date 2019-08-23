//
// Created by ilya on 16.07.19.
//

#include <iostream>
#include <string>

bool IsPalindrom(std::string &str) {

  bool res = true;
  for (int i = 0, j = str.size() - 1; i < str.size(); ++i, --j) {

    if (str[i] != str[j]) {
      res = false;
    }

    if (i - j == 1 || i == j) {
      break;
    }
  }

  return res;
}

bool isPalindrom(std::string s) {
  for (size_t i = 0; i < s.size() / 2; ++i) {
    if (s[i] != s[s.size() - i - 1]) {
      return false;
    }
  }
  return true;
}

int main() {

  std::string str;
  std::cin >> str;
  std::cout << std::boolalpha << IsPalindrom(str) << std::endl;
  return 0;
}