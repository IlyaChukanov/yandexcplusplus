//
// Created by ilya on 16.07.19.
//

#include <iostream>

int Factorial(int n) {

  if (n <= 0) {
    return 1;
  }

  int fac = 1;
  for (int i = 1; i <= n; i++) {
    fac *= i;
  }

  return fac;
}

int main() {

  int n;
  std::cin >> n;
  std::cout << Factorial(n) << std::endl;
  return 0;
}