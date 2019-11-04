//
// Created by ilya on 07.10.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_5TH_WEEK_1_TASK_JOSEPHUSPERMUTATION_H_
#define YANDEXCPLUSPLUS_3_RED_5TH_WEEK_1_TASK_JOSEPHUSPERMUTATION_H_

#include <utility>
#include <list>

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
  std::list<typename RandomIt::value_type> pool;
  std::move(first, last, back_inserter(pool));
  auto it = pool.begin();
  while (!pool.empty()) {
    *(first++) = std::move(*it);
    it = pool.erase(it);
    it--;
    for (size_t i = 0; i < step_size; ++i) {
      it++;
      if (it == pool.end()) {
        it = pool.begin();
      }
    }
    if (pool.empty()) {
      break;
    }
  }
}
#endif //YANDEXCPLUSPLUS_3_RED_5TH_WEEK_1_TASK_JOSEPHUSPERMUTATION_H_
