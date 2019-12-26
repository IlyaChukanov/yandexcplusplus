//
// Created by ilya on 16.07.19.
//

#include <vector>
#include <iostream>

std::vector<int> Reversed(const std::vector<int>& v) {
  std::vector<int> res;
  for (int i = v.size() - 1; i >= 0; --i) {
    res.push_back(v[i]);
  }
  return res;
}

/*
int main() {
  int n;
  std::cin >> n;
  std::vector<int> arr;
  for (int i = 0; i < n; ++i) {
    int buff;
    std::cin >> buff;
    arr.push_back(buff);
  }
  for (auto i : arr) {
    std::cout << i << " ";
  }
  arr = Reversed(arr);
  std::cout << std::endl;
  for (auto i : arr) {
    std::cout << i << " ";
  }
}
 */