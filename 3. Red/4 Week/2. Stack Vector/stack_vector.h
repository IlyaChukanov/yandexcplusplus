//
// Created by ilya on 04.10.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_4RD_WEEK_2_TASK_STACKVECTOR_H_
#define YANDEXCPLUSPLUS_3_RED_4RD_WEEK_2_TASK_STACKVECTOR_H_

#include <stdexcept>
#include <array>

template <typename T, size_t N>
class StackVector {
 public:
  explicit StackVector(size_t a_size = 0) : capacity_(N) {
    if (a_size > capacity_) {
      throw std::invalid_argument("");
    }
    size_ = a_size;
  }

  T& operator[](size_t index) {
    return data_[index];
  }
  const T& operator[](size_t index) const {
    return data_[index];
  }

  typename std::array<T, N>::iterator begin() {
    return data_.begin();
  }
  typename std::array<T, N>::iterator end() {
    return data_.begin() + size_;
  }
  typename std::array<T, N>::const_iterator begin() const {
    return data_.begin();
  }
  typename std::array<T, N>::const_iterator end() const {
    return data_.begin() + size_;
  }

  size_t Size() const {
    return size_;
  }

  size_t Capacity() const {
    return capacity_;
  }

  void PushBack(const T& value) {
    if (size_ >= capacity_) {
      throw std::overflow_error("StackVector is full. PushBack can not be completed");
    }
    data_[size_++] = value;
  }

  T PopBack() {
    if (size_ <= 0) {
      throw std::underflow_error("StackVector is empty. PopBack can not be completed");
    }
    return data_[--size_];
  }

 private:
  std::array<T, N> data_;
  size_t size_;
  size_t capacity_;
};


#endif //YANDEXCPLUSPLUS_3_RED_4RD_WEEK_2_TASK_STACKVECTOR_H_
