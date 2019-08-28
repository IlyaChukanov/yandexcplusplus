//
// Created by ilya on 28.08.2019.
//

#include <iostream>
#include <map>

using namespace std;

template <typename Key, typename Value>
Value& GetRefStrict(std::map<Key, Value>& map, const Key& key) {
  if (!map.count(key)) {
    throw std::runtime_error("");
  }
  return map.at(key);
}

int main() {
  map<int, string> m = {{0, "value"}};
  string& item = GetRefStrict(m, 0);
  item = "newvalue";
  cout << m[0] << endl; // выведет newvalue
}
