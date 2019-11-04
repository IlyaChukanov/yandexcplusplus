//
// Created by ilya on 13.10.2019.
//

#include "../../../MyUtils/MyTestFramework/TestFramework.h"

#include <map>
#include <set>
#include <string>
#include <future>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cmath>
#include <thread>

template <typename Iterator>
class Page {
 public:
  Page(Iterator first, Iterator second) : begin_(first), end_(second) {}

  Iterator begin() {
    return begin_;
  }

  Iterator begin() const {
    return begin_;
  }

  Iterator end() {
    return end_;
  }

  Iterator end() const {
    return end_;
  }

  size_t size() const {
    return std::distance(begin_, end_);
  }

 private:
  Iterator begin_, end_;
};

template <typename Iterator>
class Paginator {
 public:

  Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = distance(begin, end); left > 0; ) {
      size_t current_page_size = std::min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      data_.push_back({begin, current_page_end});

      left -= current_page_size;
      begin = current_page_end;
    }
  }

  auto begin() {
    return data_.begin();
  }

  auto begin() const {
    return data_.begin();
  }

  auto end() {
    return data_.end();
  }

  auto end() const {
    return data_.end();
  }

  size_t size() const {
    return data_.size();
  }

 private:
  std::vector<Page<Iterator>> data_;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator(c.begin(), c.end(), page_size);
}

struct Stats {
  std::map<std::string, int> word_frequences;
  void operator += (const Stats& other) {
    for (const auto& [word, freq] : other.word_frequences) {
      word_frequences[word] += freq;
    }
  }
};

std::vector<std::string> SplitToString(std::istream& input) {
  std::vector<std::string> result;
  std::string line;
  while (input >> line) {
    result.push_back(line);
  }
  return result;
}

Stats ExploreKeyWordsSingleThread(const std::set<std::string>& key_words, Page<std::vector<std::string>::iterator>&& input) {
  Stats result;
  for (const auto& i : input) {
    if ( key_words.find(i) != key_words.end()) {
      result.word_frequences[i] += 1;
    }
  }
  return result;
}

Stats ExploreKeyWords(const std::set<std::string>& key_words, std::istream& input) {
  Stats main_stat;
  const size_t THREAD_COUNT = (std::thread::hardware_concurrency() == 0) ? 4 : std::thread::hardware_concurrency();
  std::vector<std::string> words;
  words = SplitToString(input);
  const size_t PAGE_SIZE = std::ceil(words.size() * 1.0 / THREAD_COUNT);
  std::vector<std::future<Stats>> futures;
  for (auto& page : Paginate(words, PAGE_SIZE)) {
    futures.push_back(std::async(ExploreKeyWordsSingleThread, std::cref(key_words), page));
  }
  for (auto& f : futures) {
    main_stat += f.get();
  }
  return main_stat;
}

void TestBasic() {
  const std::set<std::string> key_words = {"yangle", "rocks", "sucks", "all"};

  std::stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const std::map<std::string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  auto i = 0;
  while(true) {
    std::cout << i++ << std::endl;
    RUN_TEST(tr, TestBasic);
  }
}
