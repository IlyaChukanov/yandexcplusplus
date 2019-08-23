//
// Created by ilya on 17.07.19.
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class SortedStrings {
public:
  void AddString(const string& s) {
    strings.push_back(s);
  }
  vector<string> GetSortedStrings() {
    std::sort(strings.begin(), strings.end());
    return strings;
  }
private:

  std::vector<std::string> strings;

};

/*
int main() {
  SortedStrings strings;

  strings.AddString("first");
  strings.AddString("third");
  strings.AddString("second");
  PrintSortedStrings(strings);

  strings.AddString("second");
  PrintSortedStrings(strings);

  return 0;
}
 */