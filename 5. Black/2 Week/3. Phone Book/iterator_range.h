//
// Created by ilya_l on 5/17/20.
//

#ifndef YANDEXCPLUSPLUS_5_BLACK_2_WEEK_3_PHONE_BOOK_ITERATOR_RANGE_H
#define YANDEXCPLUSPLUS_5_BLACK_2_WEEK_3_PHONE_BOOK_ITERATOR_RANGE_H

#include <algorithm>

template <typename It>
class IteratorRange {
public:
  IteratorRange(It first, It last) : first(first), last(last) {
  }

  It begin() const {
    return first;
  }

  It end() const {
    return last;
  }

  size_t size() const {
    return last - first;
  }

private:
  It first, last;
};

template <typename Container>
auto Head(Container& c, int top) {
  return IteratorRange(std::begin(c), std::begin(c) + std::min<size_t>(std::max(top, 0), c.size()));
}

#endif //YANDEXCPLUSPLUS_5_BLACK_2_WEEK_3_PHONE_BOOK_ITERATOR_RANGE_H
