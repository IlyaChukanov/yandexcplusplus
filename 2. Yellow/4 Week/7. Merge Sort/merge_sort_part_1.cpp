//
// Created by ilya on 10.09.2019.
//

#include <vector>
#include <algorithm>

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  if (range_end - range_begin < 2) {
    return;
  }
  std::vector<typename RandomIt::value_type> new_vec(range_begin, range_end);
  vector<typename RandomIt::value_type>
  MergeSort(new_vec.begin(), new_vec.begin() + new_vec.size() / 2);
  MergeSort(new_vec.begin() + new_vec.size() / 2, new_vec.end());

  std::merge(new_vec.begin(), new_vec.begin() + new_vec.size() / 2,
             new_vec.begin() + new_vec.size() / 2, new_vec.end(),
             range_begin);
}
