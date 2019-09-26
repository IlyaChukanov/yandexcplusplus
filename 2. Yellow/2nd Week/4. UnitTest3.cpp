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

class Rational {
 public:
  Rational() {
    num = 0;
    denom = 1;
    positive = true;
  }

  Rational(int numerator, int denominator) {
    int gcd = GCD(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
    ((double(numerator) / denominator) < 0) ? positive = false : positive = true;
    num = abs(numerator);
    denom = abs(denominator);

  }

  int Numerator() const {
    return positive ? num : -num;
  }

  int Denominator() const {
    return denom;
  }

 private:

  int GCD(int first, int second) {
    while (second) {
      first %= second;
      int buff = first;
      first = second;
      second = buff;
    }
    return first;
  }

  int num;
  int denom;
  bool positive;
};

UnitTestList TestConstructor() {
  UnitTestList us("Constructor test");
  us.AddTest(
      UnitTest(
          TestName("Default constructor"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r;
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 0;
            answer.second = 1;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = 1, q = 3"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(1, 3);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 1;
            answer.second = 3;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = 2, q = 6"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(2, 6);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 1;
            answer.second = 3;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = 3, q = 9"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(3, 9);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 1;
            answer.second = 3;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = 5, q = 20"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(5, 20);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 1;
            answer.second = 4;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = 7, q = 30"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(7, 30);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 7;
            answer.second = 30;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = 6, q = 8"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(6, 8);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 3;
            answer.second = 4;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  return us;
}

UnitTestList TestPositive() {
  UnitTestList us("Positive test");
  us.AddTest(
      UnitTest(
          TestName("Constructor p = -1, q = -3"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(-1, -3);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 1;
            answer.second = 3;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = -6, q = -8"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(-6, -8);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 3;
            answer.second = 4;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = -5, q = -38"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(-5, -38);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 5;
            answer.second = 38;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  return us;
}

UnitTestList TestNegative() {
  UnitTestList us("Negative test");
  us.AddTest(
      UnitTest(
          TestName("Constructor p = 6, q = -8"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(6, -8);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = -3;
            answer.second = 4;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = -6, q = 8"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(-6, 8);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = -3;
            answer.second = 4;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = -5, q = 11"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(-5, 11);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = -5;
            answer.second = 11;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Constructor p = 5, q = -11"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(5, -11);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = -5;
            answer.second = 11;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  return us;
}

UnitTestList TestZero() {
  UnitTestList us("Zero Test");
  us.AddTest(
      UnitTest(
          TestName("Constructor p = 0, q = 11"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Rational r(0, 11);
            std::pair<int, int> result, answer;
            result.first = r.Numerator();
            result.second = r.Denominator();
            answer.first = 0;
            answer.second = 1;
            Assert(result, answer, Equal<std::pair<int, int>, std::pair<int, int>>, desc);
          })
  );
  return us;
}

void Tests() {
  bool is_class = false;
  if (is_class) {
    UnitTestManager um;
    um.Add(TestConstructor());
    um.Add(TestPositive());
    um.Add(TestNegative());
    um.Add(TestZero());
    um.Run();
  }
  else {
    int fail_count = 0;
    auto result_construcor_test = TestConstructor().RunTests();
    fail_count += result_construcor_test.second;

    auto result_positive_test = TestPositive().RunTests();
    fail_count += result_positive_test.second;

    auto result_negative_test = TestNegative().RunTests();
    fail_count += result_negative_test.second;

    auto result_zero_test = TestZero().RunTests();
    fail_count += result_zero_test.second;

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
