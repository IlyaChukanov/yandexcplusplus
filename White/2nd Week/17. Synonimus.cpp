//
// Created by ilya on 17.07.19.
//

#include <string>
#include <set>
#include <map>
#include <iostream>

int main() {
  std::map<std::string, std::set<std::string>> syn;
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::string request;
    std::cin >> request;

    if (request == "ADD") {
      std::string word1, word2;
      std::cin >> word1 >> word2;
      syn[word1].insert(word2);
      syn[word2].insert(word1);
    }

    if (request == "COUNT") {
      std::string word;
      std::cin >> word;
      std::cout << syn[word].size() << std::endl;
    }

    if (request == "CHECK") {
      std::string word1, word2;
      std::cin >> word1 >> word2;
      if (syn[word1].count(word2)) {
        std::cout << "YES" << std::endl;
      }
      else {
        std::cout << "NO" << std::endl;
      }
    }

  }
  return 0;
}