//#include "../../../MyUtils/MyTestFramework/TestFramework.h"
//#include "priority_collection.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <map>
#include <set>
#include <vector>

using namespace std;

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
      *ids_begin = Add(std::move(*i));
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
class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
*/