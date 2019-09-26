//
// Created by ilya on 26.08.2019.
//

#include <vector>
#include <iostream>

int64_t AverageTem(const std::vector<int> &arr) {
  int64_t sum = 0;
  for (auto i : arr) {
    sum += i;
  }
  sum /= static_cast<int64_t>(arr.size());
  return sum;
}

std::vector<int> MoreThanAverage(const std::vector<int> &arr) {
  int64_t average = AverageTem(arr);
  std::vector<int> res;
  for (size_t i = 0; i < arr.size(); ++i) {
    if (arr[i] > average) res.push_back(i);
  }
  return res;
}

int main() {
  int q;
  std::cin >> q;
  std::vector<int> arr(q);
  for (auto &i : arr) {
    std::cin >> i;
  }
  auto res = MoreThanAverage(arr);
  std::cout << res.size() << std::endl;
  for (auto i : res) {
    std::cout << i << " ";
  }
  return 0;
}
