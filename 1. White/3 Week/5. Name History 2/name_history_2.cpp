//
// Created by ilya on 20.08.19.
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
  void ChangeFirstName(int year, const string& first_name) {
    first_names[year] = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
    last_names[year] = last_name;
  }
  string GetFullName(int year) {
    // получаем имя и фамилию по состоянию на год year
    const string first_name = FindNameByYear(first_names, year);
    const string last_name = FindNameByYear(last_names, year);

    // если и имя, и фамилия неизвестны
    if (first_name.empty() && last_name.empty()) {
      return "Incognito";

      // если неизвестно только имя
    } else if (first_name.empty()) {
      return last_name + " with unknown first name";

      // если неизвестна только фамилия
    } else if (last_name.empty()) {
      return first_name + " with unknown last name";

      // если известны и имя, и фамилия
    } else {
      return first_name + " " + last_name;
    }
  }

  string GetFullNameWithHistory(int year) {
    std::string first_name;
    std::string last_name;
    std::stack<std::string> f_names;
    std::stack<std::string> s_names;

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
    } else if (first_name.empty()) {
      if (!res_l.empty()) res_l += " ";
      return last_name + " " + res_l + "with unknown first name";
    } else if (last_name.empty()) {
      if (!res_f.empty()) res_f += " ";
      return first_name + " " + res_f + "with unknown last name";
    } else {
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
};

int main() {
  Person person;

  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1990, "Polina");
  person.ChangeLastName(1990, "Volkova-Sergeeva");
  cout << person.GetFullNameWithHistory(1990) << endl;

  person.ChangeFirstName(1966, "Pauline");
  cout << person.GetFullNameWithHistory(1966) << endl;

  person.ChangeLastName(1960, "Sergeeva");
  for (int year : {1960, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeLastName(1961, "Ivanova");
  cout << person.GetFullNameWithHistory(1967) << endl;

  return 0;
}

