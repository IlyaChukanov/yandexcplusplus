//
// Created by ilya on 29.09.2019.
//

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>

class Learner {
 private:
  std::set<std::string> dict;

 public:
  int Learn(const std::vector<std::string>& words) {
    int newWords = 0;
    for (const auto& word : words) {
      auto insert = dict.insert(word);
      if (insert.second) {
        newWords++;
      }
    }
    return newWords;
  }

  std::vector<std::string> KnownWords() {
    return {dict.begin(), dict.end()};
  }
};

int main() {
  Learner learner;
  std::string line;
  while (getline(std::cin, line)) {
    std::vector<std::string> words;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word) {
      words.push_back(word);
    }
    std::cout << learner.Learn(words) << "\n";
  }
  std::cout << "=== known words ===\n";
  for (auto word : learner.KnownWords()) {
    std::cout << word << "\n";
  }
}