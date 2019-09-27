//
// Created by ilya on 27.09.2019.
//

#include <vector>
#include <stdexcept>
#include <iostream>

template <typename T>
class Deque {
 public:
  Deque() = default;
  bool Empty() const {
    return front_.empty() && back_.empty();
  }

  size_t Size() const {
    return front_.size() + back_.size();
  }

  T& operator[](size_t idx) {
    if (idx >= front_.size()) {
      idx %= front_.size();
      return back_[idx];
    }
    else {
      return front_[idx];
    }
  }

  const T& operator[](size_t idx) const {
    if (idx >= front_.size()) {
      idx %= front_.size();
      return back_[idx];
    }
    else {
      return front_[idx];
    }
  }

  T& At(size_t idx) {
    if (idx >= (front_.size() + back_.size())) {
      throw std::out_of_range("");
    }
    else {
      return this->operator[](idx);
    }
  }

  const T& At(size_t idx) const {
    if (idx >= (front_.size() + back_.size())) {
      throw std::out_of_range("");
    }
    else {
      return this->operator[](idx);
    }
  }

  T& Front() {
    return front_.front();
  }

  const T& Front() const {
    return front_.front();
  }

  T& Back() {
    return back_.back();
  }

  const T& Back() const {
    return back_.back();
  }

  void PushFront(const T& obj) {
    front_.insert(front_.begin(), obj);
  }

  void PushBack(const T& obj) {
    back_.push_back(obj);
  }

 private:

  std::vector<T> front_;
  std::vector<T> back_;
};

int main() {
  Deque<int> d;
  d.PushFront(1);
  d.PushFront(2);
  d.PushFront(3);
  d.PushBack(4);
  d.PushBack(5);
  d.PushBack(6);
  std::cout << d[0] << std::endl;
  std::cout << d[1] << std::endl;
  std::cout << d[2] << std::endl;
  std::cout << d[3] << std::endl;
  std::cout << d[4] << std::endl;
  std::cout << d[5] << std::endl;
  std::cout << d[13] << std::endl;
}