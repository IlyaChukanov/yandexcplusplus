//
// Created by ilya on 03.10.2019.
//



#include <algorithm>
#include <numeric>
#include <vector>

template <typename T>
void Swap(T* first, T* second) {
  std::swap(*first, *second);
}

template <typename T>
void SortPointers(std::vector<T*>& pointers) {
  std::sort(pointers.begin(), pointers.end(), [](const auto& lhs, const auto& rhs) { return *lhs < *rhs;});
}

template <typename T>
void ReversedCopy(T* source, size_t count, T* destination) {
  T* source_begin = source;
  T* source_end = source + count;
  T* dest_begin = destination;
  T* dest_end = dest_begin + count;
  if (dest_begin >= source_end || dest_end <= source_begin) {
    std::reverse_copy(source_begin, source_end, dest_begin);
  }
  else if (dest_begin > source_begin) {
    for (size_t i = 0; source_begin + i < dest_begin; ++i) {
      *(dest_end - i - 1) = *(source_begin + i);
    }
    std::reverse(dest_begin, source_end);
  }
  else {
    for (size_t i = 0; source_end - i - 1 >= dest_end; ++i) {
      *(dest_begin + i) = *(source_end - i - 1);
    }
    std::reverse(source_begin, dest_end);
  }
}
/*
void TestSwap() {
  int a = 1;
  int b = 2;
  Swap(&a, &b);
  ASSERT_EQUAL(a, 2);
  ASSERT_EQUAL(b, 1);

  std::string h = "world";
  std::string w = "hello";
  Swap(&h, &w);
  ASSERT_EQUAL(h, "hello");
  ASSERT_EQUAL(w, "world");
}

void TestSortPointers() {
  int one = 1;
  int two = 2;
  int three = 3;

  std::vector<int*> pointers;
  pointers.push_back(&two);
  pointers.push_back(&three);
  pointers.push_back(&one);

  SortPointers(pointers);

  ASSERT_EQUAL(pointers.size(), 3u);
  ASSERT_EQUAL(*pointers[0], 1);
  ASSERT_EQUAL(*pointers[1], 2);
  ASSERT_EQUAL(*pointers[2], 3);
}

void TestReverseCopy() {
  const size_t count = 7;

  int* source = new int[count];
  int* dest = new int[count];

  for (size_t i = 0; i < count; ++i) {
    source[i] = i + 1;
  }
  ReversedCopy(source, count, dest);
  const std::vector<int> expected1 = {7, 6, 5, 4, 3, 2, 1};
  ASSERT_EQUAL(std::vector<int>(dest, dest + count), expected1);

  ReversedCopy(source, count - 1, source + 1);
  const std::vector<int> expected2 = {1, 6, 5, 4, 3, 2, 1};
  ASSERT_EQUAL(std::vector<int>(source, source + count), expected2);

  int* source1 = new int[count * 2];
  for (size_t i = 0; i < count * 2; ++i) {
    source1[i] = i + 1;
  }
  ReversedCopy(source1, count - 1, source1 + count);
  const std::vector<int> expected3 = {1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 14};
  ASSERT_EQUAL(std::vector<int>(source1, source1 + count*2), expected3);
  delete[] dest;
  delete[] source;
  delete[] source1;

}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSwap);
  RUN_TEST(tr, TestSortPointers);
  RUN_TEST(tr, TestReverseCopy);
  return 0;
}*/