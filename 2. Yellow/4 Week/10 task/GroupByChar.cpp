//
// Created by ilya on 12.09.2019.
//

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>

template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(
    RandomIt range_begin, RandomIt range_end,
    char prefix) {
  std::string str;
  str.push_back(prefix);
  auto c = std::equal_range(range_begin, range_end, str, [prefix](const std::string& lhs, const std::string& rhs) { return lhs[0] < rhs[0]; });
  return c;
}

int main() {
  const std::vector<std::string> sorted_strings = {"moscow",  "murmansk", "vologda"};

  const auto m_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
  for (auto it = m_result.first; it != m_result.second; ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  const auto p_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
  std::cout << (p_result.first - begin(sorted_strings)) << " " <<
       (p_result.second - begin(sorted_strings)) << std::endl;

  const auto z_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
  std::cout << (z_result.first - begin(sorted_strings)) << " " <<
       (z_result.second - begin(sorted_strings)) << std::endl;

  return 0;
}