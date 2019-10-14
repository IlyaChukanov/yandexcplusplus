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

using namespace std;

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

  }

  Access operator[](const K& key) {

  }

  std::map<K, V> BuildOrdinaryMap() {

  }

 private:
  const size_t BUCKET_COUNT;
  const size_t KEY_STEP = 10'000;

  struct Interval {
    size_t map_index;
    int64_t border;
  };

  std::vector<Interval> map_indexes_;
  std::vector<std::map<K, V>> maps_;
  std::vector<std::mutex> maps_mutex;
  bool is_unsigned = false;

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
    // Интервалы вида -inf...N*(-KEY_STEP)...2*(-KEY_STEP)...1*(-KEY_STEP)...1/2*(-KEY_STEP)..0..1/2*KEY_STOP...1*(-KEY_STEP)...2*(-KEY_STEP)...N*(-KEY_STEP)...+inf
    // Различаются для четного и нечетного BUCKET_COUNT


    if (BUCKET_COUNT == 1) {
      map_indexes_.push_back(Interval{{BUCKET_COUNT - 1, std::numeric_limits<K>::max()}});
    }
    else {
      // negative_count - количество map для отрицательного диапазона чисел
      // middle count - в случае если число map нечетное
      // positive_count - количество map для не отрицательного диапазона чисел
      std::array<size_t, 3> counts = {BUCKET_COUNT / 2, BUCKET_COUNT / 2, BUCKET_COUNT % 2};

      for (size_t)

    }
  }

  void SplitRange(int& neg_count, int& pos_count) {

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
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
}