//
// Created by ilya on 27.09.2019.
//

#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>

#define LOG(name) \
//std::cerr << #name << std::endl

template <typename T>
class Deque {
 public:
  Deque() = default;
  bool Empty() const {
    LOG(empty);
    return front_.empty() && back_.empty();
  }

  size_t Size() const {
    LOG(size);
    return front_.size() + back_.size();
  }

  T& operator[](size_t idx) {
    LOG(op);
    if (idx >= front_.size()) {
      idx -= front_.size();
      return back_[idx];
    }
    else {
      return front_[front_.size() - 1 - idx];
    }
  }

  const T& operator[](size_t idx) const {
    LOG(opcon);
    if (idx >= front_.size()) {
      idx %= front_.size();
      return back_[idx];
    }
    else {
      return front_[front_.size() - 1 - idx];
    }
  }

  T& At(size_t idx) {
    LOG(at);
    if (idx >= (front_.size() + back_.size())) {
      throw std::out_of_range("");
    }
    else {
      return this->operator[](idx);
    }
  }

  const T& At(size_t idx) const {
    LOG(atconst);
    if (idx >= (front_.size() + back_.size())) {
      throw std::out_of_range("");
    }
    else {
      return this->operator[](idx);
    }
  }

  T& Front() {
    LOG(front);
    if (front_.empty()) {
      return back_.front();
    }
    else {
      return front_.back();
    }
  }

  const T& Front() const {
    LOG(frontconst);
    if (front_.empty()) {
      return back_.front();
    }
    else {
      return front_.back();
    }
  }

  T& Back() {
    LOG(back);
    if (back_.empty()) {
      return front_.front();
    }
    else {
      return back_.back();
    }
  }

  const T& Back() const {
    LOG(backconst);
    if (back_.empty()) {
      return front_.front();
    }
    else {
      return back_.back();
    }
  }

  void PushFront(const T& obj) {
    LOG(pushf);
    front_.push_back(obj);
  }

  void PushBack(const T& obj) {
    LOG(pushb);
    back_.push_back(obj);
  }

 private:
  std::vector<T> front_;
  std::vector<T> back_;
};

#define PRINT(x) std::cout << x << std::endl

int main() {
  Deque<int> d;
  d.PushBack(1);
  d.PushBack(2);
  d.PushBack(3);
  d.PushFront(4);
  d.PushFront(5);
  PRINT(d.Front());
  PRINT(d.Back());
  PRINT("--");
  PRINT(d[0]);
  PRINT(d[1]);
  PRINT(d[2]);
  PRINT(d[3]);
  PRINT(d[4]);
}