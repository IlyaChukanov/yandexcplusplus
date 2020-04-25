//
// Created by ilya on 05.10.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_4RD_WEEK_4_TASK_AIRPORTCOUNTER_H_
#define YANDEXCPLUSPLUS_3_RED_4RD_WEEK_4_TASK_AIRPORTCOUNTER_H_

#include <algorithm>
#include <array>
#include <iostream>
#include <random>

// TAirport should be enum with sequential items and last item TAirport::Last_
template <typename TAirport>
class AirportCounter {
 public:
  // конструктор по умолчанию: список элементов пока пуст
  AirportCounter() {
    stat_.fill(0);
  }

  // конструктор от диапазона элементов типа TAirport
  template <typename TIterator>
  AirportCounter(TIterator begin, TIterator end) {
    stat_.fill(0);
    for (auto i = begin; i != end; ++i) {
      stat_[static_cast<uint32_t>(*i)] += 1;
    }
  }

  // получить количество элементов, равных данному
  size_t Get(TAirport airport) const {
    return stat_[static_cast<uint32_t>(airport)];
  }

  // добавить данный элемент
  void Insert(TAirport airport) {
    stat_[static_cast<uint32_t>(airport)] += 1;
  }

  // удалить одно вхождение данного элемента
  void EraseOne(TAirport airport) {
    stat_[static_cast<uint32_t>(airport)] -= 1;
  }

  // удалить все вхождения данного элемента
  void EraseAll(TAirport airport) {
    stat_[static_cast<uint32_t>(airport)] = 0;
  }

  using Item = std::pair<TAirport, size_t>;
  using Items = std::array<std::pair<TAirport, int64_t>, static_cast<uint32_t>(TAirport::Last_)>;

  // получить некоторый объект, по которому можно проитерироваться,
  // получив набор объектов типа Item - пар (аэропорт, количество),
  // упорядоченных по аэропорту
  Items GetItems() const {
    Items result;
    for (size_t i = 0; i < stat_.size(); ++i) {
      result[i] = std::make_pair(static_cast<TAirport>(i), stat_[i]);
    }
    return result;
  }

 private:
  // stat_[0] = TAirport::Item1
  // stat_[1] = TAirport::Item2 и так далее
  std::array<int64_t, static_cast<uint32_t>(TAirport::Last_)> stat_;
};

#endif //YANDEXCPLUSPLUS_3_RED_4RD_WEEK_4_TASK_AIRPORTCOUNTER_H_
