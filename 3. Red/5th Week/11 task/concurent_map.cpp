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

using namespace std;

struct Interval {
  int64_t left_border_;
  int64_t right_border;
};

template <typename K, typename V>
struct PartMap {
  PartMap() = default;

  std::map<K, V> map_;
  std::mutex m_;
};

bool operator<(const Interval& lhs, const Interval& rhs) {
  return lhs.left_border_ < rhs.right_border;
}

template <typename K>
bool operator<(const Interval& rhs, K value) {
  return value <= rhs.right_border && value < rhs.left_border_;
}

template <typename K>
bool operator<(K value, const Interval& rhs) {
  return value <= rhs.right_border && value < rhs.left_border_;
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
    if (is_unsigned) CreateUnsignedMap();
    else CreateSignedMap();
  }

  Access operator[](const K& key) {

  }

  std::map<K, V> BuildOrdinaryMap() {

  }

 private:
  const size_t BUCKET_COUNT;
  const uint64_t KEY_STEP = 10'000;
  bool is_unsigned = false;
  std::map<Interval, PartMap<K, V>, std::less<>> all_maps;

  void CreateUnsignedMap() {
    // Установка левых границ для всех интервалов исключая последний
    for (size_t i = 1; i < BUCKET_COUNT; ++i) {
      all_maps[Interval{i - 1 * KEY_STEP, i * KEY_STEP}] = std::move(PartMap<K, V>());
    }
    // Установка максимальной левой границы для последнего интервала
    all_maps.insert({Interval{BUCKET_COUNT - 1 * KEY_STEP, std::numeric_limits<K>::max()}, {}});
  }

  void CreateSignedMap() {
    if (BUCKET_COUNT == 1) {
      all_maps.insert({Interval{std::numeric_limits<K>::min(), std::numeric_limits<K>::max()}, {}});
    }
    else {
      size_t side_count = BUCKET_COUNT / 2, middle_count = BUCKET_COUNT % 2;
      int64_t start_step = 0;
      if (middle_count) {
        start_step = KEY_STEP / 2;
        all_maps.insert({Interval{-KEY_STEP, KEY_STEP}, {}});
      }
      for (size_t i = 1; i < side_count; ++i) {
        all_maps.insert({Interval{start_step + (KEY_STEP * i), start_step + (KEY_STEP * (i + 1))}, {}});
        all_maps.insert({Interval{(-start_step) + (-1 * KEY_STEP * i + 1), (-start_step) + (-1 * KEY_STEP * i)}, {}});
      }
      all_maps.insert({Interval{start_step + (KEY_STEP * (side_count)), std::numeric_limits<K>::max()}, {}});
      all_maps.insert({Interval{std::numeric_limits<K>::min(), start_step + (-1 * KEY_STEP * side_count)}, {}});
    }
  }

  /*
  std::set<Interval> map_indexes_;
  std::vector<std::map<K, V>> maps_;
  std::vector<std::mutex> maps_mutex;


  void CreateUnsignedMap() {
    maps_.resize(BUCKET_COUNT);
    maps_mutex.resize(BUCKET_COUNT);
    // Установка левых границ для всех интервалов исключая последний
    for (size_t i = 1; i < BUCKET_COUNT; ++i) {
      map_indexes_.push_back(Interval{i - 1, i * KEY_STEP});
    }
    // Установка максимальной левой границы для последнего интервала
    map_indexes_.push_back(Interval{{BUCKET_COUNT - 1, std::numeric_limits<K>::max()}});
  }

  void CreateSignedMap() {
    maps_.resize(BUCKET_COUNT);
    maps_mutex.resize(BUCKET_COUNT);
    if (BUCKET_COUNT == 1) {
      map_indexes_.insert(Interval{{BUCKET_COUNT - 1, std::numeric_limits<K>::max()}});
    }
    else {
      size_t negative_count = BUCKET_COUNT / 2, positive_count = BUCKET_COUNT / 2, middle_count = BUCKET_COUNT % 2;
    }
  }

  void CreateSignedMap() {
    maps_.resize(BUCKET_COUNT);
    maps_mutex.resize(BUCKET_COUNT);
    // Интервалы вида -inf...N*(-KEY_STEP)...2*(-KEY_STEP)...1*(-KEY_STEP)...1/2*(-KEY_STEP)..0..1/2*KEY_STOP...1*(-KEY_STEP)...2*(-KEY_STEP)...N*(-KEY_STEP)...+inf
    // Различаются для четного и нечетного BUCKET_COUNT

    if (BUCKET_COUNT == 1) {
      map_indexes_.push_back(Interval{{BUCKET_COUNT - 1, std::numeric_limits<K>::max()}});
    }
    else {
      // negative_count - количество map для отрицательного диапазона чисел
      // middle count - в случае если число map нечетное
      // positive_count - количество map для не отрицательного диапазона чисел
      size_t negative_count = BUCKET_COUNT / 2, positive_count = BUCKET_COUNT / 2, middle_count = BUCKET_COUNT % 2;
      float coef = (middle_count) ? 1.5 : 1.0;
      size_t index_interval = 0;
      for (size_t i = 0; i < negative_count; ++i) {
        map_indexes_.push_back(Interval{{index_interval++, -KEY_STEP * coef * (negative_count - i)}});
      }
      if (middle_count) {
        map_indexes_.push_back(Interval{{index_interval++, -KEY_STEP / 2}});
        map_indexes_.push_back(Interval{{index_interval++, +KEY_STEP / 2}});
      }
      else {
        map_indexes_.push_back(Interval{{index_interval++, 0}});
      }

    }
  }
   */
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
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
}