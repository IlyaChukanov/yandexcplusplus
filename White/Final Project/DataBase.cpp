//
//
// Created by ilya on 23.08.19.

#include <string>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <tuple>

using namespace std;

class Date {
 public:
  Date();
  explicit Date(const std::string& input_date);

  int GetYear() const;
  int GetMonth() const;
  int GetDay() const;

  Date& operator=(const Date& rhs);

  std::string GetDate(char separator = '-') const;

 private:
  int year;
  int month;
  int day;

  // Проверка формата
  void CheckFormat(const std::string& input);
  bool CheckNumber(stringstream& ss, int& number);
  // Проверка корректности даты
  void CheckData();
};

Date::Date() {
  year = 0;
  month = 0;
  day = 0;
}

Date::Date(const std::string &input_date) {

  CheckFormat(input_date);
  CheckData();

  if (false) {
    std::cout << "=======" << std::endl;
    std::cout << input_date << std::endl;
    std::cout << "ALL'S GOOD" << std::endl;
    std::cout << GetDate() << std::endl;
    std::cout << "=======" << std::endl;
  }

}

int Date::GetYear() const { return year; }

int Date::GetMonth() const { return month; }

int Date::GetDay() const { return day; }

Date& Date::operator=(const Date& rhs) {
  year = rhs.year;
  month = rhs.month;
  day = rhs.day;
  return *this;
}

std::string Date::GetDate(char separator) const {
  stringstream so;
  so << setfill('0');
  if (year < 0) {
    so << '-';
  }
  so << setw(4) << abs(year);
  so << separator;
  so << setw(2) << month;
  so << separator;
  so << setw(2) << day;
  return so.str();
}


void Date::CheckFormat(const std::string &input) {
  stringstream ss(input);
  int count_data = 3;
  int date_data[3] = {-1, -1, -1};
  year = date_data[0];
  month = date_data[1];
  day = date_data[2];

  if (!CheckNumber(ss, date_data[0])) {
    stringstream so;
    so << "Wrong date format: " << input;
    throw runtime_error(so.str());
  }

  char input_date_separator = '-';
  for (int i = 1; i < count_data; ++i) {
    if (ss.peek() != input_date_separator) {
      stringstream so;
      so << "Wrong date format: " << input;
      throw runtime_error(so.str());
    }
    else {
      ss.ignore(1);
      if (!CheckNumber(ss, date_data[i])) {
        stringstream so;
        so << "Wrong date format: " << input;
        throw runtime_error(so.str());
      }
    }
  }

  if (ss.rdbuf()->in_avail() != 0) {
    stringstream so;
    so << "Wrong date format: " << input;
    throw runtime_error(so.str());
  }
  year = date_data[0];
  month = date_data[1];
  day = date_data[2];
}

bool Date::CheckNumber(stringstream& ss, int& number) {
  char sign = '+';
  if (ss.peek() == '+' || ss.peek() == '-') {
    sign = ss.get();
  }
  if (ss.peek() >= '0' && ss.peek() <= '9') {
    ss >> number;
    if (sign == '-') {
      number = -number;
    }
  }
  else {
    return false;
  }
  return true;
}

void Date::CheckData() {
  if (month < 1 || month > 12) {
    stringstream ss;
    ss << "Month value is invalid: " << month;
    throw runtime_error(ss.str());
  }

  if (day < 1 || day > 31) {
    stringstream ss;
    ss << "Day value is invalid: " << day;
    throw runtime_error(ss.str());
  }
}

bool operator<(const Date& lhs, const Date& rhs) {
  //return (lhs.GetYear() < rhs.GetYear() && lhs.GetMonth() < rhs.GetMonth() && lhs.GetDay() < rhs.GetDay());
  int l_year = lhs.GetYear(), l_month = lhs.GetMonth(), l_day = lhs.GetDay();
  int r_year = rhs.GetYear(), r_month = rhs.GetMonth(), r_day = rhs.GetDay();
  return (std::tie(l_year, l_month, l_day) < std::tie(r_year, r_month, r_day));
}

bool operator>(const Date& lhs, const Date& rhs) {
  return (lhs.GetYear() > rhs.GetYear() && lhs.GetMonth() < rhs.GetMonth() && lhs.GetDay() < rhs.GetDay());
}

bool operator==(const Date& lhs, const Date& rhs) {
  return (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() == rhs.GetDay());
}

bool operator!=(const Date& lhs, const Date& rhs) {
  return (lhs.GetYear() != rhs.GetYear() && lhs.GetMonth() != rhs.GetMonth() && lhs.GetDay() != rhs.GetDay());
}

class Database {
 public:
  void AddEvent(const Date& date, const string& event);
  bool DeleteEvent(const Date& date, const string& event);
  int  DeleteDate(const Date& date);

  std::set<string> Find(const Date &date) const;
  void Print() const;

 private:
  std::map<Date, std::set<std::string>> data_;
};

void Database::AddEvent(const Date &date, const string &event) {
  data_[date].insert(event);
}

bool Database::DeleteEvent(const Date &date, const string &event) {
  try {
    auto b = data_.at(date).erase(event);
    if (b) return true;
    else return false;
  }
  catch (exception& ex) {
    return false;
  }
}

int Database::DeleteDate(const Date &date) {
  int result = 0;
  try {
    result = data_.at(date).size();
    data_.erase(date);
    return result;
  }
  catch (exception& ex) {
    return result;
  }
}

std::set<string> Database::Find(const Date &date) const {
  std::set<std::string> result;
  try {
    result = data_.at(date);
    return result;
  }
  catch (exception& ex) {
    return result;
  }
}

void Database::Print() const {
  for (const auto& events : data_) {
    for (const auto& event : events.second) {
      std::cout << events.first.GetDate() << " " << event << std::endl;
    }
  }
}

void Add(const std::string& raw_date, const std::string& event, Database& db) {
  try {
    Date date(raw_date);
    db.AddEvent(date, event);
  }
  catch (exception& ex) {
    std::cout << ex.what() << std::endl;
    return;
  }
}

// TODO Удаляет событие, даже если его нет. Исправить.
void Del(const std::string& raw_date, const std::string& event, Database& db) {
  Date date;
  try {
    Date ex_date(raw_date);
    date = ex_date;
  }
  catch (exception& ex) {
    std::cout << ex.what() << std::endl;
    return;
  }

  bool result = db.DeleteEvent(date, event);
  if (result) {
    std::cout << "Deleted successfully" << std::endl;
  }
  else {
    std::cout << "Event not found" << std::endl;
  }
}

void DelDate(const std::string& raw_date, Database& db){
  Date date;
  try {
    Date ex_date(raw_date);
    date = ex_date;
  }
  catch (exception& ex) {
    std::cout << ex.what() << std::endl;
    return;
  }
  int count = db.DeleteDate(date);
  std::cout << "Deleted " << count << " events" << std::endl;
}

void Find(const std::string& raw_date, Database& db) {
  Date date;
  try {
    Date ex_date(raw_date);
    date = ex_date;
  }
  catch (exception& ex) {
    std::cout << ex.what() << std::endl;
    return;
  }
  std::set<std::string> result = db.Find(date);
  if (!result.empty()) {
    for (const auto& event : result) {
      std::cout << event << std::endl;
    }
  }
}

void Print(Database& db) {
  db.Print();
}

int main() {
  Database db;

  string command;
  Date a;
  while (getline(std::cin, command)) {
    if (command.empty()) continue;
    stringstream ss(command);
    std::string command_name;
    ss >> command_name;
    if (command_name == "Add") {
      std::string raw_date, event;
      ss >> raw_date >> event;
      Add(raw_date, event, db);
    }
    else if (command_name == "Del") {
      std::string raw_date, event;
      ss >> raw_date >> event;
      if (event.empty()) {
        DelDate(raw_date, db);
      }
      else {
        Del(raw_date, event, db);
      }
    }
    else if (command_name == "Find") {
      std::string raw_date;
      ss >> raw_date;
      Find(raw_date, db);
    }
    else if (command_name == "Print") {
      Print(db);
    }
    else {
      std::cout << "Unknown command: " << command_name << std::endl;
    }
  }
  return 0;
}