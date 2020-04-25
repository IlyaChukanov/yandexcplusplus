//
// Created by ilya on 28.08.2019.
//

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <sstream>


template <typename T>
T Sqr(const T &value);
template <typename first, typename second>
std::pair<first, second> operator*(const std::pair<first, second>& lhs, const std::pair<first, second>& rhs);
template <typename key, typename value>
std::map<key, value> operator*(const std::map<key, value>& lhs, const std::map<key, value>& rhs);
template <typename T>
std::vector<T> operator*(const std::vector<T>& lhs, const std::vector<T> rhs);

/*
template <typename Collection>
std::string Join(const Collection& c, char d);
template <typename First, typename Second>
std::ostream& operator << (std::ostream& out, const std::pair<First, Second>& p);
template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vi);
template <typename Key, typename Value>
std::ostream& operator << (std::ostream& out, const std::map<Key, Value>& m);


template <typename Collection>
std::string Join(const Collection& c, char d) {
  std::stringstream ss;
  bool first = true;
  for (const auto& i : c) {
    if (!first) {
      ss << d;
    }
    first = false;
    ss << i;
  }
  return ss.str();
}

template <typename First, typename Second>
std::ostream& operator << (std::ostream& out, const std::pair<First, Second>& p) {
  return out << '(' << p.first << ',' << p.second << ')';
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vi) {
  return out << '[' << Join(vi, ',') << ']';
}

template <typename Key, typename Value>
std::ostream& operator << (std::ostream& out, const std::map<Key, Value>& m) {
  return out << '{' << Join(m, ',') << '}';
}
*/
template <typename T>
T Sqr(const T &value) {
  return value * value;
}

template <typename first, typename second>
std::pair<first, second> operator*(const std::pair<first, second>& lhs, const std::pair<first, second>& rhs) {
  return std::make_pair(lhs.first * rhs.first, lhs.second * rhs.second);
}

// Перемножает значения если map равны
template <typename key, typename value>
std::map<key, value> operator*(const std::map<key, value>& lhs, const std::map<key, value>& rhs) {
  if (lhs != rhs) {
    throw std::invalid_argument("Maps are not equal");
  }
  std::map<key, value> result;
  for (const auto& [k, v] : lhs) {
    result[k] = v * v;
  }
  return result;
}

template <typename T>
std::vector<T> operator*(const std::vector<T>& lhs, const std::vector<T> rhs) {
  if (lhs.size() != rhs.size()) {
    throw std::invalid_argument("Vectors are not equal");
  }
  std::vector<T> result;
  result.reserve(lhs.size());
  for (size_t i = 0; i < lhs.size(); ++i) {
    result.push_back(lhs[i] * rhs[i]);
  }
  return result;
}
/*
int main() {
  std::vector<int> a = {1, 2, 3, 4};
  std::map<int, int> b = {{1, 2}, {2, 3}, {3, 4}, {4, 5}};
  std::pair<int, int> c = {2, 3};
  std::cout << a * a << std::endl;
  std::cout << b * b << std::endl;
  std::cout << c * c << std::endl;
}
*/