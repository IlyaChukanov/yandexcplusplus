//
// Created by ilya on 21.08.19.
//
#include <map>
#include <string>
#include <stack>
#include <iostream>

using namespace std;

// если имя неизвестно, возвращает пустую строку
string FindNameByYear(const map<int, string>& names, int year) {
  string name;  // изначально имя неизвестно

  // перебираем всю историю по возрастанию ключа словаря, то есть в хронологическом порядке
  for (const auto& item : names) {
    // если очередной год не больше данного, обновляем имя
    if (item.first <= year) {
      name = item.second;
    } else {
      // иначе пора остановиться, так как эта запись и все последующие относятся к будущему
      break;
    }
  }

  return name;
}

class Person {
public:

  Person(const std::string& first_name, const std::string& last_name, int year) {
    birthday_date = year;
    first_names[year] = first_name;
    last_names[year] = last_name;
  }

  void ChangeFirstName(int year, const string& first_name) {
    if (year >= birthday_date)
      first_names[year] = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
    if (year >= birthday_date)
      last_names[year] = last_name;
  }
  string GetFullName(int year) const {
    const string first_name = FindNameByYear(first_names, year);
    const string last_name = FindNameByYear(last_names, year);

    if (year < birthday_date || (first_name.empty() && last_name.empty())) {
      return "No person";
    } else if (first_name.empty()) {
      return last_name + " with unknown first_ name";
    } else if (last_name.empty()) {
      return first_name + " with unknown last name";
    } else {
      return first_name + " " + last_name;
    }
  }

  string GetFullNameWithHistory(int year) const {
    std::string first_name;
    std::string last_name;
    std::stack<std::string> f_names;
    std::stack<std::string> s_names;

    if (year < birthday_date) {
      return "No person";
    }

    // перебираем всю историю по возрастанию ключа словаря, то есть в хронологическом порядке
    for (const auto& item : first_names) {
      // если очередной год не больше данного, обновляем имя
      if (item.first <= year) {
        if (f_names.empty() || f_names.top() != item.second)
          f_names.push(item.second);
      } else {
        // иначе пора остановиться, так как эта запись и все последующие относятся к будущему
        break;
      }
    }

    // перебираем всю историю по возрастанию ключа словаря, то есть в хронологическом порядке
    for (const auto& item : last_names) {
      // если очередной год не больше данного, обновляем имя
      if (item.first <= year) {
        if (s_names.empty() || s_names.top() != item.second)
          s_names.push(item.second);
      } else {
        // иначе пора остановиться, так как эта запись и все последующие относятся к будущему
        break;
      }
    }
    std::string res_f;
    if (!f_names.empty()) {
      first_name = f_names.top();
      f_names.pop();

      while (f_names.size()) {
        res_f += f_names.top() + ", ";
        f_names.pop();
      }
      if (!res_f.empty()) {
        res_f.pop_back();
        res_f.pop_back();
        res_f = "(" + res_f + ")";
      }
    }
    std::string res_l;
    if (!s_names.empty()) {
      last_name = s_names.top();
      s_names.pop();

      while (s_names.size()) {
        res_l += s_names.top() + ", ";
        s_names.pop();
      }
      if (!res_l.empty()) {
        res_l.pop_back();
        res_l.pop_back();
        res_l = "(" + res_l + ")";
      }
    }

    if (first_name.empty() && last_name.empty()) {
      return "Incognito";
    }
    else if (first_name.empty()) {
      if (!res_l.empty()) res_l += " ";
      return last_name + " " + res_l + "with unknown first_ name";
    }
    else if (last_name.empty()) {
      if (!res_f.empty()) res_f += " ";
      return first_name + " " + res_f + "with unknown last name";
    }
    else {
      std::string result;
      if (res_f.empty())
        result += first_name + " " + last_name;
      else
        result += first_name + " " + res_f + " " + last_name;
      if (res_l.empty())
        return result;
      return result + " " + res_l;
    }
  }

private:
  map<int, string> first_names;
  map<int, string> last_names;
  int birthday_date;
};

int main() {
  Person person("Polina", "Sergeeva", 1960);
  for (int year : {1959, 1960}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1965, "Appolinaria");
  person.ChangeLastName(1967, "Ivanova");
  for (int year : {1965, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  return 0;
}

