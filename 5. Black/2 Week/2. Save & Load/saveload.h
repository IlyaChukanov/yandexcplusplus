//
// Created by ilya_l on 5/12/20.
//

#ifndef YANDEXCPLUSPLUS_5_BLACK_2_WEEK_2_SAVE_LOAD_SAVELOAD_H
#define YANDEXCPLUSPLUS_5_BLACK_2_WEEK_2_SAVE_LOAD_SAVELOAD_H

#include <map>
#include <iostream>
#include <string>
#include <vector>

// Serialization

template <typename T>
void Serialize(T pod, std::ostream& out);
void Serialize(const std::string& str, std::ostream& out);
template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out);
template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out);
template <typename T>
void Deserialize(std::istream& in, T& pod);
void Deserialize(std::istream& in, std::string& str);
template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data);
template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data);


template <typename T>
void Serialize(T pod, std::ostream& out) {
  out.write(reinterpret_cast<const char*>(&pod), sizeof(pod));
}

void Serialize(const std::string& str, std::ostream& out) {
  Serialize(str.size(), out);
  out.write(reinterpret_cast<const char*>(str.data()), str.size());
}

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
  Serialize(data.size(), out);
  for (const auto& element : data) {
    Serialize(element, out);
  }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
  Serialize(data.size(), out);
  for (const auto& pair : data) {
    Serialize(pair.first, out);
    Serialize(pair.second, out);
  }
}

// Deserialization
template <typename T>
void Deserialize(std::istream& in, T& pod) {
  in.read(reinterpret_cast<char*>(&pod), sizeof(pod));
}

void Deserialize(std::istream& in, std::string& str) {
  size_t size;
  Deserialize(in, size);
  str.resize(size);
  if (size > 0) {
    in.read(reinterpret_cast<char*>(&str[0]), size);
  }
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
  size_t size = 0;
  Deserialize(in, size);
  data.resize(size);
  for (size_t i = 0; i < size; ++i) {
    Deserialize(in,data[i]);
  }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
  size_t size = 0;
  Deserialize(in, size);
  for (size_t i = 0; i < size; ++i) {
    T1 key;
    T2 value;
    Deserialize(in, key);
    Deserialize(in, value);
    data[key] = value;
  }
}

#endif //YANDEXCPLUSPLUS_5_BLACK_2_WEEK_2_SAVE_LOAD_SAVELOAD_H
