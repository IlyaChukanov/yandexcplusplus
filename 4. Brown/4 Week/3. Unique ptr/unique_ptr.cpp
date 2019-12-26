#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include <cstddef>  // нужно для nullptr_t
#include <algorithm>
#include <iostream>
using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
  T* raw_pointer_;
public:
  UniquePtr() {
    raw_pointer_ = nullptr;
  }

  UniquePtr(T* ptr) {
    raw_pointer_ = ptr;
  }

  UniquePtr(const UniquePtr&) = delete;

  UniquePtr(UniquePtr&& other) {
    raw_pointer_ = other.Release();
  }

  UniquePtr& operator = (const UniquePtr&) = delete;

  UniquePtr& operator = (nullptr_t) {
    Reset(nullptr);
    return *this;
  }

  UniquePtr& operator = (UniquePtr&& other) {
    raw_pointer_ = other.Release();
    return *this;
  }

  ~UniquePtr() {
    delete raw_pointer_;
  }

  T& operator*() const {
    return *Get();
  }

  T* operator->() const {
    return Get();
  }

  T * Release() {
    auto curr = raw_pointer_;
    raw_pointer_ = nullptr;
    return curr;
  }

  void Reset(T* ptr) {
    delete raw_pointer_;
    raw_pointer_ = ptr;
  }

  void Swap(UniquePtr& other) {
    std::swap(raw_pointer_, other.raw_pointer_);
  }

  T* Get() const {
    return raw_pointer_;
  }
};


struct Item {
  static int counter;
  int value;
  Item(int v = 0): value(v) {
    ++counter;
  }
  Item(const Item& other): value(other.value) {
    ++counter;
  }
  ~Item() {
    --counter;
  }
};

int Item::counter = 0;


void TestLifetime() {
  Item::counter = 0;
  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    ptr.Reset(new Item);
    ASSERT_EQUAL(Item::counter, 1);
  }
  ASSERT_EQUAL(Item::counter, 0);

  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    auto rawPtr = ptr.Release();
    ASSERT_EQUAL(Item::counter, 1);

    delete rawPtr;
    ASSERT_EQUAL(Item::counter, 0);
  }
  ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
  UniquePtr<Item> ptr(new Item(42));
  ASSERT_EQUAL(ptr.Get()->value, 42);
  ASSERT_EQUAL((*ptr).value, 42);
  ASSERT_EQUAL(ptr->value, 42);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLifetime);
  RUN_TEST(tr, TestGetters);
}
