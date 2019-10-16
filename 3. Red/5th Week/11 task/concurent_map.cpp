//
// Created by ilya on 13.10.2019.
//

#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include "../../../MyUtils/Profiler/profiler.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <random>
#include <mutex>
#include <future>
#include <set>
#include <deque>

using namespace std;

template <typename K>
struct Interval {
  Interval(const K& left_border, const K& right_border) : left_border_(left_border), right_border_(right_border) {}
  K left_border_;
  K right_border_;
};

template <typename K, typename V>
struct PartMap {
  PartMap(std::mutex& m) : m_(m) {}

  std::map<K, V> map_;
  std::mutex& m_;
};

template <typename K>
bool operator<(const Interval<K>& lhs, const Interval<K>& rhs) {
  return lhs.left_border_ < rhs.right_border_;
}

template <typename K>
bool operator<(const Interval<K>& rhs, K value) {
  return rhs.right_border_ <= value && rhs.left_border_ < value;;
}

template <typename K>
bool operator<(K value, const Interval<K>& rhs) {
  return value <= rhs.right_border_ && value < rhs.left_border_;
}

template <typename K, typename V>
class ConcurrentMap {
 public:
  static_assert(std::is_integral_v<K>, "ConcurrentMap supports only integer keys");

  struct Access {
    V& ref_to_value;
    std::lock_guard<std::mutex> guard;
  };

  explicit ConcurrentMap(size_t bucket_count) : BUCKET_COUNT(bucket_count) {
    if (!std::numeric_limits<K>::is_signed) {
      is_unsigned = true;
    }
    mutexes_.resize(BUCKET_COUNT);
    if (is_unsigned) CreateUnsignedMap();
    else CreateSignedMap();
  }

  Access operator[](const K& key) {
    auto part_map = all_maps.end();
    {
      std::lock_guard(this->am_mutex);
      part_map = all_maps.find(key);
    }
    if (part_map == all_maps.end()) {
      int a = 5;
    }
    std::lock_guard(part_map->second.m_);
    auto find_key = part_map->second.map_.find(key);
    auto& map = part_map->second.map_;
    if (find_key == part_map->second.map_.end()) {
      auto insert = part_map->second.map_.insert(std::make_pair(key, V()));
      return Access{insert.first->second, std::lock_guard(part_map->second.m_)};
    }
    else {
      return Access{find_key->second, std::lock_guard(part_map->second.m_)};
    }
  }

  std::map<K, V> BuildOrdinaryMap() {
    std::map<K, V> result;
    for (auto& [interval, map] : all_maps) {
      std::lock_guard(map.m_);
      for (const auto& [key, value] : map.map_) {
        result[key] = value;
      }
    }
    return result;
  }

 private:
  const size_t BUCKET_COUNT;
  const K KEY_STEP = 10'000;
  bool is_unsigned = false;
  std::mutex am_mutex;
  std::map<Interval<K>, PartMap<K, V>, std::less<>> all_maps;
  std::deque<std::mutex> mutexes_;

  void CreateUnsignedMap() {
    // Установка левых границ для всех интервалов исключая последний
    for (size_t i = 1; i < BUCKET_COUNT; ++i) {
      all_maps.insert({Interval<K>{i - 1 * KEY_STEP, i * KEY_STEP}, PartMap<K, V>(mutexes_[i - 1])});
    }
    // Установка максимальной левой границы для последнего интервала
    all_maps.insert({Interval<K>{BUCKET_COUNT - 1 * KEY_STEP, std::numeric_limits<K>::max()}, PartMap<K, V>(mutexes_[BUCKET_COUNT - 1])});
  }

  void CreateSignedMap() {
    if (BUCKET_COUNT == 1) {
      all_maps.insert({Interval{std::numeric_limits<K>::min(), std::numeric_limits<K>::max()}, PartMap<K, V>(mutexes_[BUCKET_COUNT - 1])});
    }
    else {
      size_t side_count = BUCKET_COUNT / 2, middle_count = BUCKET_COUNT % 2;
      K start_step = 0;
      size_t curr_mutex = 0;
      if (middle_count) {
        start_step = KEY_STEP / 2;
        all_maps.insert({Interval<K>{-start_step, start_step}, PartMap<K, V>(mutexes_[curr_mutex++])});
      }
      for (size_t i = 0; i < side_count - 1; ++i) {
        all_maps.insert({Interval<K>{start_step + (KEY_STEP * i), start_step + (KEY_STEP * (i + 1))}, PartMap<K, V>(mutexes_[curr_mutex++])});
        all_maps.insert({Interval<K>{(-start_step) + (-1 * KEY_STEP * (i + 1)), (-start_step) + (-1 * KEY_STEP * i)}, PartMap<K, V>(mutexes_[curr_mutex++])});
      }
      all_maps.insert({Interval<K>{start_step + (KEY_STEP * (side_count - 1)), std::numeric_limits<K>::max()}, PartMap<K, V>(mutexes_[curr_mutex++])});
      all_maps.insert({Interval<K>{std::numeric_limits<K>::min(), -start_step + (-1 * KEY_STEP * (side_count - 1))}, PartMap<K, V>(mutexes_[curr_mutex++])});
    }
  }
};

void RunConcurrentUpdates(
    ConcurrentMap<int, int>& cm, size_t thread_count, int key_count
) {
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);
    iota(begin(updates), end(updates), -key_count / 2);
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i) {
      for (auto key : updates) {
        cm[key].ref_to_value++;
      }
    }
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i) {
    futures.push_back(async(kernel, i));
  }
}

void TestConcurrentUpdate() {
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  const auto result = cm.BuildOrdinaryMap();
  ASSERT_EQUAL(result.size(), key_count);
  for (auto& [k, v] : result) {
    AssertEqual(v, 6, "Key = " + to_string(k));
  }
}

void TestReadAndWrite() {
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i) {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i) {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

  for (auto f : {&r1, &r2}) {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup() {
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestConcurrentUpdate);
  //RUN_TEST(tr, TestReadAndWrite);
  //RUN_TEST(tr, TestSpeedup);
}