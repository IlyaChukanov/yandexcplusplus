
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

class Person {

  struct PersonData {
    std::string FirstName;
    std::string SecondName;
  };

 public:
  void ChangeFirstName(int year, const string& first_name) {
    UpdateDate(year);
    if (data.count(year)) {
      data[year].FirstName = first_name;
    }
    else {
      PersonData new_person;
      new_person.FirstName = first_name;
      new_person.SecondName = "No data";
      data[year] = new_person;
    }
  }

  void ChangeLastName(int year, const string& last_name) {
    UpdateDate(year);
    if (data.count(year)) {
      data[year].SecondName = last_name;
    }
    else {
      PersonData new_person;
      new_person.FirstName = "No data";
      new_person.SecondName = last_name;
      data[year] = new_person;
    }
  }

  string GetFullName(int year) {
    if (year < start_date) return "Incognito";
    bool first = true, second = true;
    std::string first_name = "No data", second_name = "No data";
    while ((first || second) && year >= start_date) {
      if (first && data.count(year)) {
        first_name = data[year].FirstName;
        if (first_name != "No data")
          first = false;
      }
      if (second && data.count(year)) {
        second_name = data[year].SecondName;
        if (second_name != "No data")
          second = false;
      }
      year--;
    }
    if (first_name == "No data" && second_name == "No data") {
      return "Incognito";
    }
    else if (first_name == "No data") {
      return second_name + " with unknown first name";
    }
    else if (second_name == "No data") {
      return first_name + " with unknown last name";
    }
    else {
      return first_name + " " + second_name;
    }
  }
 private:

  void UpdateDate(int year) {
    if (year < start_date || start_date == -1)
      start_date = year;
  }

  std::string CorrectOutput(int year) {
    auto person_data = data[year];
    if (person_data.FirstName == "No data" && person_data.SecondName == "No data") {
      return "Incognito";
    }
    else if (person_data.FirstName == "No data") {
      return person_data.SecondName + " with unknown first name";
    }
    else if (person_data.SecondName == "No data") {
      return person_data.FirstName + " with unknown last name";
    }
    else {
      return person_data.FirstName + person_data.SecondName;
    }
  }
  int start_date = -1;
  std::map<int, PersonData> data;
};

UnitTestList TestIncognito() {
  UnitTestList us("Incognito test");
  us.AddTest(
      UnitTest(
          TestName("1. No history, year = 1900 "),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            const std::string result = p.GetFullName(1900);
            const std::string answer = "Incognito";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("2. No history, year = 1950"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            const std::string result = p.GetFullName(1950);
            const std::string answer = "Incognito";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("3. No history, year = -5"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            const std::string result = p.GetFullName(-5);
            const std::string answer = "Incognito";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("4. History start 1950, year = 1945"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1950, "A");
            p.ChangeLastName(1950, "FA");
            p.ChangeFirstName(1955, "B");
            const std::string result = p.GetFullName(1945);
            const std::string answer = "Incognito";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("5. History start 1950, year = 1949"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1950, "A");
            p.ChangeLastName(1950, "FA");
            p.ChangeFirstName(1955, "B");
            const std::string result = p.GetFullName(1949);
            const std::string answer = "Incognito";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  return us;
}

UnitTestList TestUpAdd() {
  UnitTestList us("Linear Up add test");
  us.AddTest(
      UnitTest(
          TestName("1. FN without LN, year = 1930"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          ""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            const std::string result = p.GetFullName(1930);
            const std::string answer = "A with unknown last name";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("2. FN without LN, year = 1935"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add FN B - 1935\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeFirstName(1935, "B");
            const std::string result = p.GetFullName(1935);
            const std::string answer = "B with unknown last name";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("3. FN without LN, year = 1934"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add FN B - 1935\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeFirstName(1935, "B");
            const std::string result = p.GetFullName(1934);
            const std::string answer = "A with unknown last name";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("4. FN without LN, year = 1937"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add FN B - 1935\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeFirstName(1935, "B");
            const std::string result = p.GetFullName(1937);
            const std::string answer = "B with unknown last name";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("5. FN with LN, year = 1930"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add SN FA - 1930\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeLastName(1930, "FA");
            const std::string result = p.GetFullName(1930);
            const std::string answer = "A FA";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("6. FN with LN, year = 1935"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add SN FA - 1930\n"
                          "Add FN B - 1935\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeLastName(1930, "FA");
            p.ChangeFirstName(1935, "B");
            const std::string result = p.GetFullName(1935);
            const std::string answer = "B FA";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("7. FN with LN, year = 1935"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add SN FA - 1930\n"
                          "Add FN B - 1935\n"
                          "Add SN FB - 1930"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeLastName(1930, "FA");
            p.ChangeFirstName(1935, "B");
            p.ChangeLastName(1935, "FB");
            const std::string result = p.GetFullName(1935);
            const std::string answer = "B FB";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("8. FN with LN, year = 1932"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add SN FA - 1930\n"
                          "Add FN B - 1935\n"
                          "Add SN FB - 1930"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeLastName(1930, "FA");
            p.ChangeFirstName(1935, "B");
            p.ChangeLastName(1935, "FB");
            const std::string result = p.GetFullName(1932);
            const std::string answer = "A FA";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("9. FN with LN, year = 1937"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add SN FA - 1930\n"
                          "Add FN B - 1935\n"
                          "Add SN FB - 1930"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeLastName(1930, "FA");
            p.ChangeFirstName(1935, "B");
            p.ChangeLastName(1935, "FB");
            const std::string result = p.GetFullName(1935);
            const std::string answer = "B FB";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("10. FN with LN, year = 1934"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add SN FA - 1930\n"
                          "Add FN B - 1935\n"
                          "Add SN FB - 1930"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeLastName(1930, "FA");
            p.ChangeLastName(1933, "FB");
            p.ChangeFirstName(1935, "B");
            p.ChangeLastName(1935, "FC");
            const std::string result = p.GetFullName(1934);
            const std::string answer = "A FB";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("11. FN with LN, year = 1937"),
          TestDescription("equal"),
          HintDescription("Add FN A - 1930\n"
                          "Add SN FA - 1930\n"
                          "Add FN B - 1935\n"
                          "Add SN FB - 1930\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1930, "A");
            p.ChangeLastName(1930, "FA");
            p.ChangeLastName(1933, "FB");
            p.ChangeFirstName(1935, "B");
            p.ChangeLastName(1935, "FC");
            const std::string result = p.GetFullName(1937);
            const std::string answer = "B FC";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("12. LN without FN, year = 1930"),
          TestDescription("equal"),
          HintDescription("Add LN FA - 1930\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeLastName(1930, "FA");
            const std::string result = p.GetFullName(1930);
            const std::string answer = "FA with unknown first name";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("13. LN without FN, year = 1935"),
          TestDescription("equal"),
          HintDescription("Add LN A - 1930\n"
                          "Add LN FB - 1935\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeLastName(1930, "FA");
            p.ChangeLastName(1935, "FB");
            const std::string result = p.GetFullName(1935);
            const std::string answer = "FB with unknown first name";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("14. LN without FN, year = 1933"),
          TestDescription("equal"),
          HintDescription("Add LN A - 1930\n"
                          "Add LN FB - 1935\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeLastName(1930, "FA");
            p.ChangeLastName(1935, "FB");
            const std::string result = p.GetFullName(1933);
            const std::string answer = "FA with unknown first name";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("15. LN without FN, year = 1937"),
          TestDescription("equal"),
          HintDescription("Add LN A - 1930\n"
                          "Add LN FB - 1935\n"),
          [](const std::string& desc) {
            Person p;
            p.ChangeLastName(1930, "FA");
            p.ChangeLastName(1935, "FB");
            const std::string result = p.GetFullName(1937);
            const std::string answer = "FB with unknown first name";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  return us;
}

UnitTestList CompleteTest() {
  UnitTestList us("Full Test");
  us.AddTest(
      UnitTest(
          TestName("1. Full test, year = 1900"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1965, "Polina");
            p.ChangeLastName(1967, "Sergeeva");
            const std::string result = p.GetFullName(1900);
            const std::string answer = "Incognito";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("2. Full test, year = 1965"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1965, "Polina");
            p.ChangeLastName(1967, "Sergeeva");
            const std::string result = p.GetFullName(1965);
            const std::string answer = "Polina with unknown last name";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("3. Full test, year = 1990"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1965, "Polina");
            p.ChangeLastName(1967, "Sergeeva");
            const std::string result = p.GetFullName(1990);
            const std::string answer = "Polina Sergeeva";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("4. Full test, year = 1969"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1965, "Polina");
            p.ChangeLastName(1967, "Sergeeva");
            p.ChangeFirstName(1970, "Appolinaria");
            const std::string result = p.GetFullName(1969);
            const std::string answer = "Polina Sergeeva";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("5. Full test, year = 1970"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1965, "Polina");
            p.ChangeLastName(1967, "Sergeeva");
            p.ChangeFirstName(1970, "Appolinaria");
            const std::string result = p.GetFullName(1970);
            const std::string answer = "Appolinaria Sergeeva";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("6. Full test, year = 1969"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1965, "Polina");
            p.ChangeLastName(1967, "Sergeeva");
            p.ChangeFirstName(1970, "Appolinaria");
            p.ChangeLastName(1968, "Volkova");
            const std::string result = p.GetFullName(1969);
            const std::string answer = "Polina Volkova";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("7. Full test, year = 1970"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Person p;
            p.ChangeFirstName(1965, "Polina");
            p.ChangeLastName(1967, "Sergeeva");
            p.ChangeFirstName(1970, "Appolinaria");
            p.ChangeLastName(1968, "Volkova");
            const std::string result = p.GetFullName(1970);
            const std::string answer = "Appolinaria Volkova";
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  return us;
}

void Tests() {
  int fail_count = 0;
  auto result_incognito_test = TestIncognito().RunTests();
  fail_count += result_incognito_test.second;

  auto result_up_add_test = TestUpAdd().RunTests();
  fail_count += result_up_add_test.second;

  auto result_full_test = CompleteTest().RunTests();
  fail_count += result_full_test.second;

  if (fail_count) {
    exit(1);
  }
}

int main() {
  Tests();
  // добавьте сюда свои тесты
  return 0;
}
