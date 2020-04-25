//
// Created by ilya on 02.10.2019.
//

//#include "../../../Utils/MyTestFramework/TestFramework.h"


#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <stdexcept>
#include <set>

template <class T>
class ObjectPool {
 public:
  T* Allocate() {
    if (free_objects_.empty()) {
      T* new_object = new T;
      detached_objects_.insert(new_object);
      return new_object;
    }
    else {
      T* old_object = free_objects_.front();
      free_objects_.pop();
      detached_objects_.insert(old_object);
      return old_object;
    }
  }

  T* TryAllocate() {
    if (free_objects_.empty()) {
      return nullptr;
    }
    else {
      T* old_object = free_objects_.front();
      free_objects_.pop();
      detached_objects_.insert(old_object);
      return old_object;
    }
  }

  void Deallocate(T* object) {
    auto find = detached_objects_.find(object);
    if (find == detached_objects_.end()) {
      throw std::invalid_argument("");
    }
    free_objects_.push(*find);
    detached_objects_.erase(find);
  }

  ~ObjectPool() {
    for (auto i = detached_objects_.begin(); i != detached_objects_.end();) {
      T* ptr = *i;
      i = detached_objects_.erase(i);
      delete ptr;
    }
    while(!free_objects_.empty()) {
      T* ptr = free_objects_.front();
      free_objects_.pop();
      delete ptr;
    }
  }

 private:
  std::set<T*> detached_objects_;
  std::queue<T*> free_objects_;
};
/*
void TestObjectPool() {
  ObjectPool<std::string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
 */