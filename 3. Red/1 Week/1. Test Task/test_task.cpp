//
// Created by ilya on 26.09.2019.
//


#include "test_runner.h"

template<typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred) {
  ForwardIterator max = last;
  for (auto curr = first; curr != last; curr++) {
    if (pred(*curr)) {
      if (max == last) max = curr;
      else if (*max < *curr) max = curr;
    }
  }
  return max;
}

