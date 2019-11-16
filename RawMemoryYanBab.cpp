#include <algorithm>
#include <cstddef>
#include <memory>
#include <new>
#include <utility>
#include <iostream>

template <typename T>
struct RawMemory {
  T* buf = nullptr;
  size_t cp = 0;

  static T* Allocate(size_t n) {
    return static_cast<T*>(operator new(n * sizeof(T)));
  }
  static void Deallocate(T* buf) {
    operator delete(buf);
  }

  void Swap(RawMemory& other) noexcept {
    std::swap(buf, other.buf);
    std::swap(cp, other.cp);
  }

  RawMemory() = default;

  RawMemory(size_t n) {
    buf = Allocate(n);
    cp = n;
  }

  RawMemory(const RawMemory&) = delete;
  RawMemory(RawMemory&& other) noexcept {
    Swap(other);
  }

  ~RawMemory() {
    Deallocate(buf);
  }

  RawMemory& operator = (const RawMemory&) = delete;
  RawMemory& operator = (RawMemory&& other) noexcept {
    Swap(other);
    return *this;
  }

  const T* operator + (size_t i) const {
    return buf + i;
  }
  T* operator + (size_t i) {
    return buf + i;
  }
  const T& operator [] (size_t i) const {
    return buf[i];
  }
  T& operator [] (size_t i) {
    return buf[i];
  }

};


template <typename T>
class Vector {
private:
  RawMemory<T> data;
  size_t sz = 0;

public:
  void Swap(Vector& other) noexcept {
    data.Swap(other.data);
    std::swap(sz, other.sz);
  }

  Vector() = default;

  Vector(size_t n): data(n) {
    std::uninitialized_value_construct_n(
      data.buf, n
    );
    sz = n;
  }

  Vector(const Vector& other): data(other.sz) {
    std::uninitialized_copy_n(
      other.data.buf, other.sz, data.buf
    );
    sz = other.sz;
  }

  Vector(Vector&& other) noexcept {
    Swap(other);
  }

  ~Vector() {
    std::destroy_n(data.buf, sz);
  }

  Vector& operator = (const Vector& other) {
    if (other.sz > data.cp) {
      Vector tmp(other);
      Swap(tmp);
    } else {
      for (size_t i = 0; i < sz && i < other.sz; ++i) {
        data[i] = other.data[i];
      }
      if (sz < other.sz) {
        std::uninitialized_copy_n(
          other.data.buf + sz,
          other.sz - sz,
          data.buf
        );
      } else if (sz > other.sz) {
        std::destroy_n(
          data.buf + other.sz,
          sz - other.sz
        );
      }
      sz = other.sz;
    }
    return *this;
  }

  Vector& operator = (Vector&& other) noexcept {
    Swap(other);
    return *this;
  }

  void Reserve(size_t n) {
    if (n > data.cp) {
      RawMemory<T> data2(n);
      std::uninitialized_move_n(
        data.buf, sz, data2.buf
      );
      std::destroy_n(data.buf, sz);
      data = std::move(data2);
    }
  }

  void Resize(size_t n) {
    Reserve(n);
    if (sz < n) {
      std::uninitialized_value_construct_n(
        data + sz, n - sz
      );
    } else if (sz > n) {
      std::destroy_n(
        data + n, sz - n
      );
    }
    sz = n;
  }

  void PushBack(const T& elem) {
    if (sz == data.cp) {
      Reserve(sz == 0 ? 1 : sz * 2);
    }
    new (data + sz) T(elem);
    ++sz;
  }

  void PushBack(T&& elem) {
    if (sz == data.cp) {
      Reserve(sz == 0 ? 1 : sz * 2);
    }
    new (data + sz) T(std::move(elem));
    ++sz;
  }

  template <typename ... Args>
  T& EmplaceBack(Args&&... args) {
    if (sz == data.cp) {
      Reserve(sz == 0 ? 1 : sz * 2);
    }
    auto elem = new (data + sz) T(std::forward<Args>(args)...);
    ++sz;
    return *elem;
  }

  void PopBack() {
    std::destroy_at(data + sz - 1);
    --sz;
  }

  size_t Size() const noexcept {
    return sz;
  }
  size_t Capacity() const noexcept {
    return data.cp;
  }

  const T& operator[](size_t i) const {
    return data[i];
  }
  T& operator[](size_t i) {
    return data[i];
  }

  using iterator = T*;
  using const_iterator = const T*;

  iterator begin() noexcept {
    return data.buf;
  }
  iterator end() noexcept {
    return data.buf + sz;
  }

  const_iterator begin() const noexcept {
    return data.buf;
  }
  const_iterator end() const noexcept {
    return data.buf + sz;
  }

  // Тут должна быть такая же реализация, как и для константных версий begin/end
  const_iterator cbegin() const noexcept {
    return data.buf;
  }
  const_iterator cend() const noexcept {
    return data.buf + sz;
  }

  void ShiftFrom(size_t idx) {
    new (data+sz) T(std::move(data[sz-1]));
    for (size_t i = sz-1; i > idx; i--) {
      data[i] = std::move(data[i-1]);
    }
  }

  // Вставляет элемент перед pos
  // Возвращает итератор на вставленный элемент
  iterator Insert(const_iterator pos, const T& elem) {
    size_t idx = pos - data.buf;
    if (data.cp == sz) {
      Reserve(sz == 0 ? 1 : sz * 2);
    }
    if (idx == sz) {
      new (data.buf+sz) T(elem);
    } else {
      T copy(elem);
      ShiftFrom(idx);
      data[idx] = std::move(copy);
    }
    ++sz;
    return data.buf + idx;
  }
  iterator Insert(const_iterator pos, T&& elem) {
    size_t idx = pos - data.buf;
    if (data.cp == sz) {
      Reserve(sz == 0 ? 1 : sz * 2);
    }
    if (idx == sz) {
      new (data+sz) T(std::move(elem));
    } else {
      ShiftFrom(idx);
      data[idx] = std::move(elem);
    }
    ++sz;
    return data.buf + idx;
  }

  // Конструирует элемент по заданным аргументам конструктора перед pos
  // Возвращает итератор на вставленный элемент
  template <typename ... Args>
  iterator Emplace(const_iterator it, Args&&... args) {
    size_t idx = it - data.buf;
    if (data.cp == sz) {
      Reserve(sz == 0 ? 1 : sz * 2);
    }
    if (idx == sz) {
      new (data.buf + sz) T(std::forward<Args>(args)...);
    } else {
      ShiftFrom(idx);
      data[idx] = T(std::forward<Args>(args)...);
    }
    ++sz;
    return data.buf + idx;
  }

  // Удаляет элемент на позиции pos
  // Возвращает итератор на элемент, следующий за удалённым
  iterator Erase(const_iterator it) {
    size_t idx = it - data.buf;
      // std::cout << idx << "," << sz << std::endl;
    for (size_t i = idx; i < sz-1; i++) {
      data[i] = std::move(data[i+1]);
    }
    data[sz-1].~T();
    --sz;
    return data.buf + idx;
  }
};
