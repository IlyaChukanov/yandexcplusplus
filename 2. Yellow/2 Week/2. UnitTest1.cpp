//#include "MyTestFramework.h"
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
    os << "Assertion failed:" << "\n{\n" << first << "\n}\n" << description << "\n{\n" << second << "\n}";
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
      exit(1);
    }
    else {
      std::cerr << list_name << " successful" << std::endl;
    }
  }

  void Test(UnitTest test, const std::string& test_name) {
    try {
      test();
      std::cerr << " -- " << test_name << " OK" << std::endl;
    } catch (std::exception& e) {
      ++fail_count;
      std::cerr << " -- " << test_name << " fail:\n" << e.what() << std::endl;
    }
  }

  void RunTest() {
    UnitTest test = unit_test_list.front();
    unit_test_list.pop();
    Test(test, test.Who());
  }

  std::pair<int, int> RunTests() {
    int good_count = unit_test_list.size();
    while (unit_test_list.size()) {
      RunTest();
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

using namespace std;

int GetDistinctRealRootCount(double A, double B, double C) {
  double D = B * B - 4 * A * C;
  if (A == 0) {
    if (B != 0) {
      return 1;
    }
    return 0;
  } else if (D == 0) {
    return 1;
  } else if (D > 0) {
    return 2;
  } else {
    return 0;
  }
}

UnitTestList TestConstant() {
  UnitTestList us("Constant test");
  us.AddTest(
      UnitTest(
          TestName("1 = 0 no root"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(0, 0, 1);
            const int answer = 0;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("-1 = 0 no root"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(0, 0, -1);
            const int answer = 0;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("123= 0 no root"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(0, 0, 123);
            const int answer = 0;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("-579 = 0 no root"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(0, 0, -579);
            const int answer = 0;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("189238910 = 0 no root"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(0, 0, 189238910);
            const int answer = 0;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  return us;
}

UnitTestList TestLinear() {
  UnitTestList us("Linear test case(a = 0)");
  us.AddTest(
      UnitTest(
          TestName("x2 + 1 = 0 one real root"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(0, 2, 1);
            const int answer = 1;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("-x = 0 one real root"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(0, -1, 0);
            const int answer = 1;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("120x - 10 = 0 one real root"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(0, 120, -10);
            const int answer = 1;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("120x = 0 one real root"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(0, 120, 0);
            const int answer = 1;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  return us;
}

UnitTestList TestRoot() {
  UnitTestList us("Root test");
  us.AddTest(
      UnitTest(
          TestName("Root testing"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {

            mt19937 gen;
            uniform_real_distribution<> unif(-10, 10);

            for (auto i = 0; i < 100; ++i) {
              const auto a = unif(gen);
              const auto b = unif(gen);
              const auto c = unif(gen);

              const int count = GetDistinctRealRootCount(a, b, c);
              Assert((0 <= count && count <= 2), true, Equal<bool, bool>, desc);
            }
          })
  );
  return us;
}

UnitTestList TestOneRoot() {
  UnitTestList us("Root test");
  us.AddTest(
      UnitTest(
          TestName("Root testing"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {

            mt19937 gen;
            uniform_real_distribution<> unif(-10, 10);

            for (auto i = 0; i < 100; ++i) {
              const auto v = unif(gen);
              const auto p = v + v;
              const auto q = v * v;

              const int count = GetDistinctRealRootCount(1, p, q);
              Assert(count, 1, Equal<int, int>, desc);
            }
          })
  );
  return us;
}

UnitTestList TestNoRoot() {
  UnitTestList us("One Root test");
  us.AddTest(
      UnitTest(
          TestName("x^2+1 = 0 has 0 real roots"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(1, 0, 1);
            const int answer = 0;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("x^2-2x+10 = 0 has 0 real roots"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(1, -2, 10);
            const int answer = 0;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("x^2+3x+4 = 0 has 0 real roots"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            const int result = GetDistinctRealRootCount(1, 3, 4);
            const int answer = 0;
            Assert(result, answer, Equal<int, int>, desc);
          })
  );
  return us;
}

void Tests() {
  TestRoot().RunTests();
  TestConstant().RunTests();
  TestLinear().RunTests();
  TestOneRoot().RunTests();
  TestNoRoot().RunTests();
}

int main() {
  Tests();
  return 0;
}
