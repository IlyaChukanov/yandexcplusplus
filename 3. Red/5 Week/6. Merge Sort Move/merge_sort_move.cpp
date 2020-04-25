//
// Created by ilya on 11.10.2019.
//
//#include "../../../Utils/MyTestFramework/TestFramework.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  if (std::distance(range_begin, range_end) < 2) {
    return;
  }
  std::vector<typename RandomIt::value_type> new_vec(std::make_move_iterator(range_begin),
                                                       std::make_move_iterator(range_end)),
                                             temp;
  MergeSort(new_vec.begin(), new_vec.begin() + new_vec.size() / 3);
  MergeSort(new_vec.begin() + new_vec.size() / 3, new_vec.begin() + new_vec.size() * 2 / 3);
  MergeSort(new_vec.begin() + new_vec.size() * 2 / 3, new_vec.end());

  std::merge(std::make_move_iterator(new_vec.begin()),
             std::make_move_iterator(new_vec.begin() + new_vec.size() / 3),
             std::make_move_iterator(new_vec.begin() + new_vec.size() / 3),
             std::make_move_iterator(new_vec.begin() + new_vec.size() * 2 / 3),
             std::back_inserter(temp));

  std::merge(std::make_move_iterator(temp.begin()),
             std::make_move_iterator(temp.end()),
             std::make_move_iterator(new_vec.begin() + new_vec.size() * 2 / 3),
             std::make_move_iterator(new_vec.end()),
             range_begin);
}
/*
void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
*/