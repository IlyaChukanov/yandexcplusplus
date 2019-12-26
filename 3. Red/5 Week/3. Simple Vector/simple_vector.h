//
// Created by ilya on 09.10.2019.
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

  void PushBack(const T& value) {
    if (!capacity_) {
      Resize(1);
    }
    else if (capacity_ == size_) {
      Resize(capacity_*2);
    }
    *end_ = value;
    size_++;
    end_++;
  }

  SimpleVector& operator=(const SimpleVector& lhs) {
    delete[] data_;
    capacity_ = lhs.capacity_;
    size_ = lhs.size_;
    data_ = new T[lhs.capacity_];
    std::copy(lhs.begin(), lhs.end(), begin());
    end_ = data_ + lhs.size_;
    return *this;
    /*
    Другая реализация

    if (rhs.size <= capacity_) {
      // У нас достаточно памяти - просто копируем элементы
      copy(rhs.begin(), rhs.end(), begin());
      size_ = rhs.size;
    } else {
      // идиома copy-and-swap.
      SimpleVector<T> tmp(rhs);
      swap(tmp.data, data_);
      swap(tmp.size, size_);
      swap(tmp.capacity, capacity_);
    }
    */
  }

 private:
  void Resize(size_t new_size) {
    T* temp_data = new T[new_size];
    size_t old_size = size_;
    capacity_ = new_size;
    for (size_t i = 0; i < size_; ++i) {
      temp_data[i] = data_[i];
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
