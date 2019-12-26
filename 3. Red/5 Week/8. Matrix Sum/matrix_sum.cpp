//
// Created by ilya on 12.10.2019.
//

#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include <vector>
#include <future>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

template <typename Iterator>
class Page {
 public:
  Page(Iterator first, Iterator second) : begin_(first), end_(second) {}

  Iterator begin() {
    return begin_;
  }

  Iterator begin() const {
    return begin_;
  }

  Iterator end() {
    return end_;
  }

  Iterator end() const {
    return end_;
  }

  size_t size() const {
    return std::distance(begin_, end_);
  }

 private:
  Iterator begin_, end_;
};

template <typename Iterator>
class Paginator {
 public:

  Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = distance(begin, end); left > 0; ) {
      size_t current_page_size = min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      data_.push_back({begin, current_page_end});

      left -= current_page_size;
      begin = current_page_end;
    }
  }

  auto begin() {
    return data_.begin();
  }

  auto begin() const {
    return data_.begin();
  }

  auto end() {
    return data_.end();
  }

  auto end() const {
    return data_.end();
  }

  size_t size() const {
    return data_.size();
  }

 private:
  std::vector<Page<Iterator>> data_;
};

template <typename Page>
int64_t CalculateSumThread(Page page) {
  int64_t result = 0;
  for (const auto& v : page) {
    for (const auto& i : v) {
      result += i;
    }
  }
  return result;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
  int64_t result = 0;
  const size_t THREAD_COUNT = 4;
  const size_t PAGE_SIZE = std::ceil(matrix.size() * 1.0 / THREAD_COUNT);
  std::vector<std::future<int64_t>> futures;
  for (auto& page : Paginator(matrix.begin(), matrix.end(), PAGE_SIZE)) {
    futures.push_back(std::async([page]{
      return CalculateSumThread(page);
    }));
  }
  for (auto& f : futures) {
    result += f.get();
  }
  return result;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9, 10, 11, 12},
      {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateSumThread(matrix.begin(), matrix.end()), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}