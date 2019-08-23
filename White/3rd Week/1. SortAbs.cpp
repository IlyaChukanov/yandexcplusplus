//
// Created by ilya on 17.07.19.
//

#include <algorithm>
#include <vector>
#include <iostream>

int main() {
  int n;
  std::cin >> n;
  std::vector<int> dig(n);
  for (auto& i : dig) {
    std::cin >> i;
  }

  std::sort(dig.begin(), dig.end(), [](int a, int b) { return abs(a) < abs(b);} );
  for (const auto& i : dig) {
    std::cout << i << " ";
  }

  return 0;
}