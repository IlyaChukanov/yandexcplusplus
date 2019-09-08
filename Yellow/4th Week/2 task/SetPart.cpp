//
// Created by ilya on 08.09.2019.
//

#include <set>
#include <iostream>
#include <algorithm>
#include <vector>

template <typename T>
std::vector<T> AnotherFindGreaterElements(const std::set<T>& elements, const T& border) {
  // Начнём итерироваться по множеству
  auto it = begin(elements);
  // Цель — найти первый элемент, больший border
  // Если итератор не достиг конца и указывает не туда, двигаем
  while (it != end(elements) && *it <= border) {
    ++it;
  }
  // Возвращаем вектор, созданный из элементов множества, начиная с it
  return {it, end(elements)};
}

template <typename T>
std::vector<T> FindGreaterElements(const std::set<T>& elements, const T& border) {
  auto border_it = std::find_if(elements.begin(), elements.end(), [border](T current){ return current > border;});
  return {border_it, elements.end()};
}

int main() {
  for (int x : FindGreaterElements(std::set<int>{1, 5, 7, 8}, 5)) {
    std::cout << x << " ";
  }
  std::cout << std::endl;

  std::string to_find = "Python";
  std::cout << FindGreaterElements(std::set<std::string>{"C", "C++"}, to_find).size() << std::endl;
  return 0;
}
