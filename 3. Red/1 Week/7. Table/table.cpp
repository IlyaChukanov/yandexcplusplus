//
// Created by ilya on 27.09.2019.
//

//#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include <vector>
#include <utility>

template <typename T>
class Table {
 private:
  std::vector<std::vector<T>> data;

 public:
  Table(size_t rows, size_t columns) {
    Resize(rows, columns);
  }

  const std::vector<T>& operator [] (size_t idx) const {
    return data[idx];
  }

  std::vector<T>& operator [] (size_t idx) {
    return data[idx];
  }

  void Resize(size_t rows, size_t columns) {
    data.resize(rows);
    for (auto& item : data) {
      item.resize(columns);
    }
  }

  std::pair<size_t, size_t> Size() const {
    return {data.size(), (data.empty() ? 0 : data[0].size())};
  }

};
/*
void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}*/