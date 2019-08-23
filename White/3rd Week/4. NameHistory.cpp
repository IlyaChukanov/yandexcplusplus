//
// Created by ilya on 13.08.19.
//
#include <string>
#include <map>
#include <iostream>

using namespace std;

// В общем случае решается через interval map
// Текущее решение основано на том, что мы по каждому параметру пробегаемся по истории
// По хорошему нужно что бы каждое состояние хранило все актуальные состояния из прошлого
// Например
/*
 * ---2---3---4---5---6
 * -------2---3---3---3
 *
 * ---2---3---4---5---6
 * -------1---1-3-3-4-4
 */
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


int main() {
  Person person;

  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullName(year) << endl;
  }

  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }

  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }

  return 0;
}