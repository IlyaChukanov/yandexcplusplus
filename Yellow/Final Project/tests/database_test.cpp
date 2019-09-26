//
// Created by ilya on 26.09.2019.
//

#include <gtest/gtest.h>

#include "database.h"
#include "condition_parser.h"


TEST(DatabaseTest, Test1) {
  Database db;
  db.Add(Date("2017-01-01"), "A");
  db.Add(Date("2017-01-01"), "B");
  db.Add(Date("2017-01-01"), "C");
  db.Add(Date("2017-01-02"), "D");

  std::stringstream input("date == 2017-01-01");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 3 entries\n"
                       "2017-01-02 D\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseTest, Test2) {
  Database db;
  db.Add(Date("2017-01-01"), "A");
  db.Add(Date("2017-01-01"), "B");
  db.Add(Date("2017-01-01"), "C");
  db.Add(Date("2017-01-02"), "D");

  std::stringstream input("date == 2017-01-01 OR event == \"A\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 3 entries\n"
                       "2017-01-02 D\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseTest, Test3) {
  Database db;
  db.Add(Date("2017-01-01"), "A");
  db.Add(Date("2017-01-01"), "B");
  db.Add(Date("2017-01-01"), "C");
  db.Add(Date("2017-01-02"), "D");

  std::stringstream input("date == 2017-01-01 AND event == \"A\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 1 entries\n"
                       "2017-01-01 B\n"
                       "2017-01-01 C\n"
                       "2017-01-02 D\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseTest, Test4) {
  Database db;
  db.Add(Date("2017-01-01"), "A");
  db.Add(Date("2017-01-01"), "B");
  db.Add(Date("2017-01-01"), "C");
  db.Add(Date("2017-01-02"), "D");

  std::stringstream input("date == 2017-01-01 AND event != \"A\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 2 entries\n"
                       "2017-01-01 A\n"
                       "2017-01-02 D\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseTest, Test5) {
  Database db;
  db.Add(Date("2017-01-01"), "A");
  db.Add(Date("2017-01-01"), "B");
  db.Add(Date("2017-01-01"), "C");
  db.Add(Date("2017-01-02"), "A");

  std::stringstream input("event == \"A\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 2 entries\n"
                       "2017-01-01 B\n"
                       "2017-01-01 C\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseTest, Test6) {
  Database db;
  db.Add(Date("2017-01-01"), "A");
  db.Add(Date("2017-01-01"), "B");
  db.Add(Date("2017-01-01"), "C");
  db.Add(Date("2017-01-02"), "A");

  std::stringstream input("event != \"A\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 2 entries\n"
                       "2017-01-01 A\n"
                       "2017-01-02 A\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseTest, Test7) {
  Database db;
  db.Add(Date("2019-01-01"), "A");
  db.Add(Date("2019-01-01"), "B");

  std::stringstream input("event == \"A\"");
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };

  {
    std::stringstream first;
    int count = db.RemoveIf(predicate);
    first << "Removed " << count << " entries" << std::endl;
    db.Print(first);
    std::string second = "Removed 1 entries\n"
                         "2019-01-01 B\n";
    ASSERT_EQ(first.str(), second.c_str());
  }
  db.Add(Date("2019-01-01"), "A");
  {
    std::stringstream first;
    db.Print(first);
    std::string second = "2019-01-01 B\n"
                         "2019-01-01 A\n";
    ASSERT_EQ(first.str(), second.c_str());
  }
}