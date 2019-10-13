//
// Created by ilya on 11.10.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_5TH_WEEK_7_TASK_PRIORITY_COLLECTION_H_
#define YANDEXCPLUSPLUS_3_RED_5TH_WEEK_7_TASK_PRIORITY_COLLECTION_H_

#include <utility>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

template <typename T>
class PriorityCollection {
 public:
  using Id = size_t;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  // id_to_objects O(1)
  // prior_to_id O(1) амортизированное, log (n) в целом
  Id Add(T object) {
    id_to_objects_.push_back({std::move(object), 0});
    prior_to_id[0].insert(id_to_objects_.size() - 1);
    return id_to_objects_.size() - 1;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  // id_to_objects log (n)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
    for (auto i = range_begin; i != range_end; ++i, ++ids_begin) {
      *ids_begin = Add(*i);
    }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  // id_to_objects O(1) - Поиск T по id и проверка на -1
  bool IsValid(Id id) const {
    return id_to_objects_[id].priority_ != -1;
  }

  // Получить объект по идентификатор
  // id_to_objects O(1) - поиск T по id
  const T& Get(Id id) const {
    return id_to_objects_[id].data_;
  }

  // Увеличить приоритет объекта на 1
  // id_to_objects O(1) - поиск приоритета id и его изменение
  // prior_to_id log (n) - поиск id в базе приоритетов
  // prior_to_id.second - O(n) поиск и удаление id
  // prior_to_id log (n) - добавление по новому приоритету, можно ли ускорить?
  void Promote(Id id) {
    int priority = id_to_objects_[id].priority_++;
    auto find_prior = prior_to_id.find(priority);
    if (find_prior != prior_to_id.end()) {
      find_prior->second.erase(find_prior->second.find(id));
    }
    prior_to_id[priority + 1].insert(id);
    if (find_prior->second.empty()) {
      prior_to_id.erase(find_prior);
    }
  }

  // Получить объект с максимальным приоритетом и его приоритет
  // prior_to_id O(1) - выбор максимального и возвращения
  std::pair<const T&, int> GetMax() const {
    return {id_to_objects_[*(prior_to_id.rbegin()->second.rbegin())].data_, prior_to_id.rbegin()->first};
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  // prior_to_id O(1) - выбор максимального
  // id_to_objects O(1) - удаление по id
  std::pair<T, int> PopMax() {
    auto iter_to_last = prior_to_id.rbegin()->second.rbegin();
    auto result = std::make_pair(std::move(id_to_objects_[*(iter_to_last)].data_),
                                 prior_to_id.rbegin()->first);

    id_to_objects_[*(iter_to_last)].priority_ = -1;
    prior_to_id.rbegin()->second.erase(prev(prior_to_id.rbegin()->second.end()));
    if (prev(prior_to_id.end())->second.empty()) {
      prior_to_id.erase(prev(prior_to_id.end()));
    }
    return result;
  }

 private:
  struct Node {
    T data_;
    int priority_;
  };
  std::map<int, std::set<Id>> prior_to_id;
  // Можно ускорить превратив map - в вектор и невалидных объектов помечая приоритет как -1
  std::vector<Node> id_to_objects_;
};


/*
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = int;

  Id Add(T object) {
    const Id new_id = objects.size();
    objects.push_back({move(object)});
    sorted_objects.insert({0, new_id});
    return new_id;
  }

  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
    while (range_begin != range_end) {
      *ids_begin++ = Add(move(*range_begin++));
    }
  }

  bool IsValid(Id id) const {
    return id >= 0 && id < objects.size() &&
        objects[id].priority != NONE_PRIORITY;
  }

  const T& Get(Id id) const {
    return objects[id].data;
  }

  void Promote(Id id) {
    auto& item = objects[id];
    const int old_priority = item.priority;
    const int new_priority = ++item.priority;
    sorted_objects.erase({old_priority, id});
    sorted_objects.insert({new_priority, id});
  }

  pair<const T&, int> GetMax() const {
    const auto& item = objects[prev(sorted_objects.end())->second];
    return {item.data, item.priority};
  }

  pair<T, int> PopMax() {
    const auto sorted_objects_it = prev(sorted_objects.end());
    auto& item = objects[sorted_objects_it->second];
    sorted_objects.erase(sorted_objects_it);
    const int priority = item.priority;
    item.priority = NONE_PRIORITY;
    return {move(item.data), priority};
  }

private:
  struct ObjectItem {
    T data;
    int priority = 0;
  };
  static const int NONE_PRIORITY = -1;

  vector<ObjectItem> objects;
  set<pair<int, Id>> sorted_objects;
};

};
*/
#endif //YANDEXCPLUSPLUS_3_RED_5TH_WEEK_7_TASK_PRIORITY_COLLECTION_H_
