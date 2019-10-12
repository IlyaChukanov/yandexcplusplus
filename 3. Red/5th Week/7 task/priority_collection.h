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
template <typename T>
class PriorityCollection {
 public:
  using Id = size_t;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    auto find = priority_data_.find(0);
    if (find == priority_data_.end()) {
      auto insert = priority_data_.insert(std::make_pair(0, std::vector<Node>())).first;
      id_to_iter_.push_back(insert);
      insert->second.push_back(Node{id_to_iter_.size() - 1, std::move(object)});
    }
    else {
      id_to_iter_.push_back(find);
      find->second.push_back(Node{id_to_iter_.size() - 1, std::move(object)});
    }
    return id_to_iter_.size() - 1;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
    for (auto cur = range_begin; cur != range_end; ++cur, ++ids_begin) {;
      *ids_begin = Add(*cur);
    }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
    return id_to_iter_[id] != priority_data_.end();
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
    return id_to_iter_[id]->second.back().data_;
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    auto curr_iter = id_to_iter_[id];
    typename std::map<int, std::vector<Node>>::iterator find = priority_data_.find(curr_iter->first + 1);
    if (find == priority_data_.end()) {
      auto insert = priority_data_.insert(std::make_pair(curr_iter->first + 1, std::vector<Node>())).first;
      insert->second.push_back(std::move(curr_iter->second.back()));
      curr_iter->second.pop_back();
      id_to_iter_[id] = insert;
    }
    else {
      find->second.push_back(std::move(curr_iter->second.back()));
      curr_iter->second.pop_back();
      id_to_iter_[id] = find;
    }
    if (priority_data_[curr_iter->first].empty()) {
      priority_data_.erase(curr_iter);
    }
  }

  // Получить объект с максимальным приоритетом и его приоритет
  std::pair<const T&, int> GetMax() const {
    return {priority_data_.end()->second.back().data_, priority_data_.end()->first};
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  std::pair<T, int> PopMax() {
    T first = std::move(priority_data_.end()->second.back().data_);
    int ind = priority_data_.end()->first;
    id_to_iter_[priority_data_.end()->second.back().id_] = priority_data_.end();
    priority_data_.end()->second.pop_back();
  }

 private:

  struct Node {
    Id id_;
    T data_;
  };

  std::vector<typename std::map<int, std::vector<Node>>::iterator> id_to_iter_;
  std::map<int, std::vector<Node>> priority_data_;
};
*/
#endif //YANDEXCPLUSPLUS_3_RED_5TH_WEEK_7_TASK_PRIORITY_COLLECTION_H_
