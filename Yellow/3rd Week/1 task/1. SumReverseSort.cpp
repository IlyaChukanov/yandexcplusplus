//
// Created by ilya on 07.09.2019.
//

//#include "1. SumReverseSort.h"
#include "sum_reverse_sort.h"
#include <algorithm>

int Sum(int x, int y) {
  return x + y;
}

string Reverse(string s) {
  for (size_t i = 0; i < s.size()/2; ++i) {
    char bufer = s[i];
    s[i] = s[s.size() - i - 1];
    s[s.size() - i - 1] = bufer;
  }
  return s;
}

void Sort(vector<int>& nums) {
  std::sort(nums.begin(), nums.end());
}
