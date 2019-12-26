#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

private:
  Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
  return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
  string name;
  int age, income;
  bool is_male;
};

vector<Person> ReadPeople(istream& input) {
  int count;
  input >> count;

  vector<Person> result(count);
  for (Person& p : result) {
    char gender;
    input >> p.name >> p.age >> p.income >> gender;
    p.is_male = gender == 'M';
  }

  return result;
}

struct StatsData {
  std::string most_popular_man_name;
  std::string most_popular_woman_name;
  std::vector<size_t> wealths;
  std::vector<Person> sorted_age;
};

std::vector<size_t> GetWealth(std::vector<Person>& people) {
  sort(people.begin(), people.end(), [](const Person& lhs, const Person& rhs) {
    return lhs.income > rhs.income;
  });

  std::vector<size_t> result;
  result.resize(people.size());
  if (!people.empty()) {
    result[0] = people[0].income;
    for (size_t i = 1; i < people.size(); ++i) {
      result[i] = result[i - 1] + people[i].income;
    }
  }
  return result;
}

template <typename Iter>
std::string GetPopularName(IteratorRange<Iter> range) {
  if (range.begin() == range.end()) {
    return "";
  } else {
    sort(range.begin(), range.end(), [](const Person& lhs, const Person& rhs) {
      return lhs.name < rhs.name;
    });
    const string* most_popular_name = &range.begin()->name;
    int count = 1;
    for (auto i = range.begin(); i != range.end(); ) {
      auto same_name_end = find_if_not(i, range.end(), [i](const Person& p) {
        return p.name == i->name;
      });
      const auto cur_name_count = std::distance(i, same_name_end);
      if (
          cur_name_count > count ||
              (cur_name_count == count && i->name < *most_popular_name)
          ) {
        count = cur_name_count;
        most_popular_name = &i->name;
      }
      i = same_name_end;
    }
    return *most_popular_name;
  }
}

StatsData CreateData() {
  StatsData data;
  std::vector<Person> people = ReadPeople(cin);
  {
    IteratorRange males{
        begin(people),
        partition(begin(people), end(people), [](const Person& p) {
          return p.is_male;
        })
    };
    IteratorRange females{males.end(), end(people)};
    data.most_popular_man_name = GetPopularName(males);
    data.most_popular_woman_name = GetPopularName(females);
  }
  data.wealths = GetWealth(people);
  sort(people.begin(), people.end(), [](const Person& lhs, const Person& rhs) {
    return lhs.age < rhs.age;
  });
  data.sorted_age = std::move(people);
  return data;
}

int main() {

  const StatsData data = CreateData();
  for (string command; cin >> command; ) {
    if (command == "AGE") {
      int adult_age;
      cin >> adult_age;
      auto adult_begin = lower_bound(
        data.sorted_age.begin(), data.sorted_age.end(), adult_age, [](const Person& lhs, int age) {
          return lhs.age < age;
        }
      );
      cout << "There are " << std::distance(adult_begin, data.sorted_age.end())
           << " adult people for maturity age " << adult_age << '\n';
    }
    else if (command == "WEALTHY") {
      size_t count;
      cin >> count;
      cout << "Top-" << count << " people have total income " << data.wealths[count - 1] << '\n';
    }
    else if (command == "POPULAR_NAME") {
      char gender;
      cin >> gender;
      std::string name = gender == 'M' ? data.most_popular_man_name : data.most_popular_woman_name;
      if (name.empty()) {
        cout << "No people of gender " << gender << '\n';
      }
      else {
        cout << "Most popular name among people of gender " << gender << " is "
             << name << '\n';
      }
    }
  }
}
