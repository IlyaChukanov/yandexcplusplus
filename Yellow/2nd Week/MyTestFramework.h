//
// Created by ilya on 03.09.2019.
//

#ifndef TESTFRAMEWORK_H_
#define TESTFRAMEWORK_H_

#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <functional>
#include <queue>

template <typename First, typename Second>
std::ostream& operator << (std::ostream& out, const std::pair<First, Second>& p) {
  return out << '(' << p.first << ',' << p.second << ')';
}

// Collection must be iterable
template <typename Collection>
std::string Join(const Collection& c, const std::string& d) {
  std::stringstream ss;
  bool first = true;
  for (const auto& i : c) {
    if (!first) {
      ss << d;
    }
    first = false;
    ss << i;
  }
  return ss.str();
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& vi) {
  return out << '[' << Join(vi, ", ") << ']';
}

template <typename Key, typename Value>
std::ostream& operator << (std::ostream& out, const std::map<Key, Value>& m) {
  return out << '{' << Join(m, ", ") << '}';
}

template <typename Value>
std::ostream& operator << (std::ostream& out, const std::set<Value>& m) {
  return out << '{' << Join(m, ", ") << '}';
}

template<class T, class U>
bool Less(const T& first, const U& second) {
  return first < second;
}

template<class T, class U>
bool More(const T& first, const U& second) {
  return first > second;
}

template<class T, class U>
bool LessOrEqual(const T& first, const U& second) {
  return first <= second;
}

template<class T, class U>
bool MoreOrEqual(const T& first, const U& second) {
  return first >= second;
}

template<class T, class U>
bool Equal(const T& first, const U& second) {
  auto b = first == second;
  return first == second;
}

template<class T, class U, class Comparator>
void Assert(const T& first, const U& second, const Comparator& comp, const std::string& description) {
  if (!comp(first, second)) {
    std::ostringstream os;
    os << std::boolalpha << "Assertion failed:" << "\n{\n" << first << "\n}\n" << "Operation: " << description << " Result: " << check << "\n{\n" << second << "\n}" ;
    throw std::runtime_error(os.str());
  }
}

class TestName {
 public:
  explicit TestName(std::string str) : test_name{str} {}
  std::string operator()() const { return test_name; }
 private:
  std::string test_name;
};

class TestDescription {
 public:
  explicit TestDescription(std::string str) : test_desc{str} {}
  std::string operator()() const { return test_desc; }
 private:
  std::string test_desc;
};

class HintDescription {
 public:
  explicit HintDescription(std::string str) : hint_desc{str} {}
  std::string operator()() const { return hint_desc; }
 private:
  std::string hint_desc;
};

class UnitTest {
 public:
  UnitTest(const TestName& test_name,
           const TestDescription& test_description,
           const HintDescription& hint_description,
           const std::function<void(std::string)>& func) : test_name_{test_name},
                                                           hint_description_{hint_description},
                                                           test_description_{test_description} {
    function_ = func;
  }

  void operator()() {
    try {
      function_(test_description_());
    }
    catch (std::exception& e) {
      std::stringstream ss;
      ss << e.what() << " " << hint_description_();
      // TODO Как избавиться от исключений?
      throw std::runtime_error(ss.str());
    }
  }

  std::string Who() const {
    return test_name_();
  }

 private:
  TestDescription test_description_;
  HintDescription hint_description_;
  TestName test_name_;
  std::function<void(std::string)> function_;
};

class UnitTestList {
 public:

  explicit UnitTestList(const std::string& tests_name = "Unnamed") {
    list_name = tests_name;
  }

  ~UnitTestList() {
    if (fail_count > 0) {
      std::cerr << list_name << " not successful" << std::endl;
      std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
      //exit(1);
    }
    else {
      std::cerr << list_name << " successful" << std::endl;
    }
  }

  std::pair<int, int> RunTests() {

    std::cerr << std::endl;
    std::cerr << list_name << " started" << std::endl;

    int good_count = unit_test_list.size();
    while (unit_test_list.size()) {
      UnitTest test = unit_test_list.front();
      unit_test_list.pop();
      try {
        test();
        std::cerr << " -- " << test.Who() << " OK" << std::endl;
      } catch (std::exception& e) {
        ++fail_count;
        std::cerr << " -- " << test.Who() << " fail:\n" << e.what() << std::endl;
      }
    }
    return std::make_pair(good_count - fail_count, fail_count);
  }

  void AddTest(const UnitTest& test) {
    unit_test_list.push(test);
  }

  std::string ListName() const { return list_name; }

 private:

  std::string list_name;
  int fail_count = 0;
  std::queue<UnitTest> unit_test_list;
};

// TODO Не работает в тестовой системе, что-то с ним не так. Не знаю что.
class UnitTestManager {
 public:
  UnitTestManager() : good_tests(0), fail_tests(0) {};
  ~UnitTestManager() {
    /*
    std::cerr << "=============================================\n"
                 "============= RESULT OF TESTING =============\n"
                 "=============================================\n";
    std::cerr << "Successful tests: " << good_tests << std::endl;
    std::cerr << "Fails test: " << fail_tests << std::endl;
    for (const auto& name : failed_test_) {
      std::cerr << name << " is failed" << std::endl;
    }
    */
    if (fail_tests > 0) {
      std::cerr << "Failed testing of programm. Terminate" << std::endl;
      exit(1);
    }
    else {
      std::cerr << "Success" << std::endl;
    }
  }

  void Add(const UnitTestList& list) {
    std::string key = std::to_string(list_.size() + 1) + ") " + list.ListName();
    list_.insert(std::make_pair(key, list));
  }

  void Run() {
    for (auto& [name, list] : list_) {
      std::cerr << "\n" << name << " started" << std::endl;
      auto result = list.RunTests();
      good_tests += result.first;
      fail_tests += result.second;
      if (!result.second) {
        std::cerr << list.ListName() << " successful" << std::endl;
      }
      else {
        failed_test_.push_back(name);
        std::cerr << list.ListName() << " unsuccessful\n" << result.second << " failed tests" << std::endl;
      }
      list_.erase(name);
    }
  }
 private:
  std::map<std::string, UnitTestList> list_;
  std::vector<std::string> failed_test_;
  int good_tests;
  int fail_tests;
};

#endif //TESTFRAMEWORK_H_
