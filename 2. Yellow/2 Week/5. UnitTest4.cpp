#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <functional>
#include <queue>
#include <random>

using namespace std;

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
  return first == second;
}

template<class T, class U>
bool NotEqual(const T& first, const U& second) {
  return first != second;
}

template<class T, class U, class Comparator>
void Assert(const T& first, const U& second, const Comparator& comp, const std::string& description) {
  bool check = comp(first, second);
  if (!check) {
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

class UnitTestManager {
 public:
  UnitTestManager() : good_tests(0), fail_tests(0) {};
  ~UnitTestManager() {
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
      auto result = list.RunTests();
      good_tests += result.first;
      fail_tests += result.second;
      list_.erase(name);
    }
  }

 private:
  std::map<std::string, UnitTestList> list_;
  int good_tests;
  int fail_tests;
};

/*
bool IsPalindrom(const std::string &str) {

  bool res = true;
  for (int i = 0, j = str.size() - 1; i < str.size(); ++i, --j) {
    if (str[i] != str[j]) {
      res = false;
    }
    if (i - j == 1 || i == j) {
      break;
    }
  }
  return res;
}
*/

UnitTestList TestBasicTrue() {
  UnitTestList us("Base true test");
  us.AddTest(
      UnitTest(
          TestName("String: abcde"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abcde"), false, Equal<bool, bool>, desc);
          })
  );

  us.AddTest(
      UnitTest(
          TestName("String: aba"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aba"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: madam"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("madam"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: level"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("level"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: wasitacaroracatisaw"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("wasitacaroracatisaw"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: aacaa"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aacaa"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: aaccaa"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aaccaa"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: aaaaa"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aaaaa"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: aaaa"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aaaa"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: aa"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aa"), true, Equal<bool, bool>, desc);
          })
  );
  return us;
}

UnitTestList TestBasicFalse() {
  UnitTestList us("Base false test");
  us.AddTest(
      UnitTest(
          TestName("String: abcde"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abcde"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: some"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("some"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: asomea"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("asomea"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("String: ac"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("ac"), false, Equal<bool, bool>, desc);
          })
  );
  return us;
}

UnitTestList TestUnusual() {
  UnitTestList us("Unusual test");
  us.AddTest(
      UnitTest(
          TestName("Empty string"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom(""), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("One space string"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom(" "), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("One char string"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("a"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <abc cba>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abc cba"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <abc abc>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abc abc"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <abcd dcba>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abc cba"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <abc   cba>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abc   cba"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <abc  cba>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abc  cba"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <abc  abc>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abc  abc"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <aba  aba>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aba  aba"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <aaa  aaa>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aaa  aaa"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: < aaa  aaa   >"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom(" aaa  aaa   "), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: < aaa  aaa >"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom(" aaa  aaa "), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <  aaa  aaa  >"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("  aaa  aaa  "), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <   aaa  aaa >"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("   aaa  aaa "), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <baa  aaa>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("baa  aaa"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <baa  aab>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("baa  aab"), true, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: <i hate testing>"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("i hate testing"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: aabba"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aabba"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: abbaa"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abbaa"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: aaabba"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("aaabbaa"), false, Equal<bool, bool>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Sentence: abbaaa"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(IsPalindrom("abbaaa"), false, Equal<bool, bool>, desc);
          })
  );
  return us;
}

void Tests() {
  bool is_class = false;
  if (is_class) {
    UnitTestManager um;
    um.Add(TestBasicTrue());
    um.Add(TestBasicFalse());
    um.Add(TestUnusual());
    um.Run();
  }
  else {
    int fail_count = 0;
    auto result_true_test = TestBasicTrue().RunTests();
    fail_count += result_true_test.second;

    auto result_false_test = TestBasicFalse().RunTests();
    fail_count += result_false_test.second;

    auto result_unusual_test = TestUnusual().RunTests();
    fail_count += result_unusual_test.second;

    if (fail_count) {
      exit(1);
    }
  }
}

int main() {
  Tests();
  // добавьте сюда свои тесты
  return 0;
}
