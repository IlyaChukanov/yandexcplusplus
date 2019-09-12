//
// Created by ilya on 12.09.2019.
//

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(
    RandomIt range_begin, RandomIt range_end,
    const std::string& prefix) {
  auto c = std::equal_range(range_begin, range_end, prefix,
      [prefix](const std::string& lhs, const std::string& rhs)
        {
          std::string pref_lhs(lhs.begin(), lhs.begin() + prefix.size());
          std::string pref_rhs(rhs.begin(), rhs.begin() + prefix.size());
          return std::string(lhs.begin(), lhs.begin() + prefix.size()) < std::string(rhs.begin(), rhs.begin() + prefix.size());
        });
  return c;
}

int main() {
  const std::vector<std::string> sorted_strings = {"moscow", "motovilikha", "murmansk"};

  const auto mo_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
  for (auto it = mo_result.first; it != mo_result.second; ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  const auto mt_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
  std::cout << (mt_result.first - begin(sorted_strings)) << " " <<
       (mt_result.second - begin(sorted_strings)) << std::endl;

  const auto na_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
  std::cout << (na_result.first - begin(sorted_strings)) << " " <<
       (na_result.second - begin(sorted_strings)) << std::endl;

  return 0;
}