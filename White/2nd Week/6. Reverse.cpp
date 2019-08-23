//
// Created by ilya on 16.07.19.
//

#include <vector>
#include <iostream>

void Swap(int &a, int &b) {
  int buff = a;
  a = b;
  b = buff;
}

void Reverse(std::vector<int> &v) {
  for (int i = 0; i < v.size() / 2; ++i) {
    Swap(v[i], v[v.size() - i - 1]);
  }
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
  Reverse(arr);
  std::cout << std::endl;
  for (auto i : arr) {
    std::cout << i << " ";
  }
}
 */