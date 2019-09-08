//
// Created by ilya on 08.09.2019.
//

#include <vector>
#include <iostream>
#include <algorithm>

void PrintVectorPart(const std::vector<int>& numbers) {
  // There is std::vector<int>::const_iterator
  auto first_negative = std::find_if(numbers.begin(), numbers.end(), [](int current) { return current < 0;});
  for (auto i = first_negative; i != numbers.begin(); ) {
    std::cout << *(--i);
    if (i != numbers.begin()) std::cout << " ";
  }
}

int main() {
  PrintVectorPart({6, 1, 8, -5, 4});
  std::cout << std::endl;
  PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
  std::cout << std::endl;
  PrintVectorPart({6, 1, 8, 5, 4});
  std::cout << std::endl;
  return 0;
}