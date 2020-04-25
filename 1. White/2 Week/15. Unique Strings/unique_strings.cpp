//
// Created by ilya on 17.07.19.
//

#include <string>
#include <set>
#include <iostream>


int main() {
  int n;
  std::cin >> n;
  std::set<std::string> uniq_str;
  for (int i = 0; i < n; ++i) {
    std::string str;
    std::cin >> str;
    uniq_str.insert(str);
  }
  std::cout << uniq_str.size() << std::endl;
  return 0;
}