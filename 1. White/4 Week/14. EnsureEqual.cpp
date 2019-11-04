//
// Created by ilya on 23.08.19.
//

#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;

void EnsureEqual(const string& left, const string& right) {
  if (left != right) {
    std::stringstream ss;
    ss << left << " != " << right;
    throw runtime_error(ss.str());
  }
}


int main() {
  try {
    EnsureEqual("C++ White", "C++ White");
    EnsureEqual("C++ White", "C++ 2. Yellow");
  } catch (runtime_error& e) {
    cout << e.what() << endl;
  }
  return 0;
}
