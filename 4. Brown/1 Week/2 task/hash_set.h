//
// Created by ilya on 04.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_1_WEEK_2_TASK_HASH_SET_H_
#define YANDEXCPLUSPLUS_4_BROWN_1_WEEK_2_TASK_HASH_SET_H_

#include <forward_list>
#include <vector>
#include <algorithm>

template <typename Type, typename Hasher>
class HashSet {
 public:
  using BucketList = std::forward_list<Type>;
  explicit HashSet(
      size_t num_buckets,
      const Hasher& hasher = {}
  ) : bucket_count_(num_buckets), hash_func_(hasher) {
    buckets_.resize(num_buckets);
  }

  void Add(const Type& value) {
    size_t ind = hash_func_(value) % bucket_count_;
    auto find = std::find(buckets_[ind].begin(), buckets_[ind].end(), value);
    if (find == buckets_[ind].end()) {
      buckets_[ind].push_front(value);
    }
  }

  bool Has(const Type& value) const {
    size_t ind = hash_func_(value) % bucket_count_;;
    auto find = std::find(buckets_[ind].begin(), buckets_[ind].end(), value);
    return find != buckets_[ind].end();
  }

  void Erase(const Type& value) {
    size_t ind = hash_func_(value) % bucket_count_;
    //auto find = std::find(buckets_[ind].begin(), buckets_[ind].end(), value);
    buckets_[ind].remove(value);
  }

  const BucketList& GetBucket(const Type& value) const {
    size_t ind = hash_func_(value) % bucket_count_;
    return buckets_[ind];
  }
 private:
  Hasher hash_func_;
  std::vector<BucketList> buckets_;
  size_t bucket_count_;
};

#endif //YANDEXCPLUSPLUS_4_BROWN_1_WEEK_2_TASK_HASH_SET_H_
