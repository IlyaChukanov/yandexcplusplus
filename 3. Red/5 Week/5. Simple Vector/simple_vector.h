//
// Created by ilya on 10.10.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_5TH_WEEK_3_TASK_SIMPLEVECTOR_H_
#define YANDEXCPLUSPLUS_3_RED_5TH_WEEK_3_TASK_SIMPLEVECTOR_H_

#include <cstdlib>
#include <algorithm>

template <typename T>
class SimpleVector {
 public:
  SimpleVector() {
    data_ = nullptr;
    end_ = nullptr;
    capacity_ = 0;
    size_ = 0;
  }
  explicit SimpleVector(size_t size) {
    data_ = new T[size];
    end_ = data_ + size;
    capacity_ = size;
    size_ = size;
  }
  ~SimpleVector() {
    delete [] data_;
  }

  T& operator[](size_t index) {
    return data_[index];
  }

  T* begin() {
    return data_;
  }
  T* end() {
    return end_;
  }
  T* begin() const {
    return data_;
  }
  T* end() const {
    return end_;
  }

  size_t Size() const {
    return size_;
  }
  size_t Capacity() const {
    return capacity_;
  }

  void PushBack(T value) {
    if (!capacity_) {
      Resize(1);
    }
    else if (capacity_ == size_) {
      Resize(capacity_*2);
    }
    *end_ = std::move(value);
    size_++;
    end_++;
  }

 private:
  void Resize(size_t new_size) {
    T* temp_data = new T[new_size];
    size_t old_size = size_;
    capacity_ = new_size;
    for (size_t i = 0; i < size_; ++i) {
      temp_data[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = temp_data;
    end_ = data_ + size_;
  }

  T* data_;
  T* end_;
  // Вместимость
  size_t capacity_;
  // Текущий размер
  size_t size_;
};

#endif //YANDEXCPLUSPLUS_3_RED_5TH_WEEK_3_TASK_SIMPLEVECTOR_H_
