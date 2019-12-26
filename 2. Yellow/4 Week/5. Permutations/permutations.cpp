//
// Created by ilya on 09.09.2019.
//

#include <iostream>
#include <algorithm>
#include <vector>

int Factorial(int n) {
  int res = 1;
  for (int i = 0; i < n; ++i) {
    res *= (i+1);
  }
  return res;
}

template <class InputIt>
void PrintRange(InputIt begin, InputIt end) {
  for (InputIt it = begin; it != end; ++it) {
    if (it == begin) std::cout << *it;
    else std::cout << " " << *it;
  }
}

void AllPermutations(int n) {
  int count = Factorial(n);
  std::vector<int> permut(n);
  for (int i = 0; i < n; ++i) {
    permut[i] = i + 1;
  }
  for (int i = 0; i < count; ++i) {
    std::prev_permutation(permut.begin(), permut.end());
    PrintRange(permut.begin(), permut.end());
    std::cout << std::endl;
  }
}

void AllPermutations1(int n) {
  std::vector<int> permut(n);
  for (int i = 0; i < n; ++i) {
    permut[i] = i + 1;
  }
  while (std::prev_permutation(permut.begin(), permut.end())) {
    PrintRange(permut.begin(), permut.end());
    std::cout << std::endl;
  }
}

int main() {
  int n = 0;
  std::cin >> n;
  AllPermutations(n);
}