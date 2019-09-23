//
// Created by ilya on 18.09.2019.
//

#include <gtest/gtest.h>

#include "database.h"
#include "condition_parser.h"



TEST(DatabaseAddPrint, SuccesTest1) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-1-1 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-1-1 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream first;
  db.Print(first);
  std::string second = "2017-01-01 Holiday\n"
                       "2017-01-01 New Year\n"
                       "2017-03-08 Holiday\n";
  EXPECT_EQ(first.str(), second.c_str());
}

TEST(DatabaseAddPrint, SuccesTest2) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-02-15 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-13 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-05-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream first;
  db.Print(first);
  std::string second = "2017-01-01 Holiday\n"
                       "2017-02-15 Holiday\n"
                       "2017-03-08 Holiday\n"
                       "2017-03-13 Holiday\n"
                       "2017-05-08 Holiday\n";

  EXPECT_EQ(first.str(), second.c_str());
}

TEST(DatabaseAddPrint, SuccesTest3) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2015-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2019-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream first;
  db.Print(first);
  std::string second = "2015-01-01 Holiday\n"
                       "2017-01-01 Holiday\n"
                       "2019-01-01 Holiday\n";
  EXPECT_EQ(first.str(), second.c_str());
}

TEST(DatabaseAddPrint, SuccesTest4) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream first;
  db.Print(first);
  std::string second = "2017-01-01 Holiday\n";
  EXPECT_EQ(first.str(), second.c_str());
}

TEST(DatabaseAddPrint, SuccesTest5) {
  Database db;
  {
    std::stringstream input("2015-12-25 Birthday 25");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday2");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2019-12-25 Birthday 29");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  {
    std::stringstream first;
    db.Print(first);
    std::string second = "2015-12-25 Birthday 25\n"
                         "2017-01-01 Holiday\n"
                         "2017-01-01 Holiday1\n"
                         "2017-01-01 Holiday2\n"
                         "2019-12-25 Birthday 29\n";
    EXPECT_EQ(first.str(), second.c_str());
  }

  {
    std::stringstream input("2000-01-01 Old epoch");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2020-01-01 New epoch");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2025-12-25 Crysis");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  {
    std::stringstream first;
    db.Print(first);
    std::string second = "2000-01-01 Old epoch\n"
                         "2015-12-25 Birthday 25\n"
                         "2017-01-01 Holiday\n"
                         "2017-01-01 Holiday1\n"
                         "2017-01-01 Holiday2\n"
                         "2019-12-25 Birthday 29\n"
                         "2020-01-01 New epoch\n"
                         "2025-12-25 Crysis\n";
    EXPECT_EQ(first.str(), second.c_str());
  }
}

TEST(DatabaseAddPrint, SuccesTest6) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday3");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 AA");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 CC");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 BB");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday2");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 A");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream first;
  db.Print(first);
  std::string second = "2017-01-01 Holiday3\n"
                       "2017-01-01 AA\n"
                       "2017-01-01 CC\n"
                       "2017-01-01 BB\n"
                       "2017-01-01 Holiday2\n"
                       "2017-01-01 Holiday1\n"
                       "2017-01-01 A\n";
  EXPECT_EQ(first.str(), second.c_str());
}

TEST(DatabaseAddPrint, ErrorTest1) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday2");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday3");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream first;
  db.Print(first);
  {
    std::string second = "2017-01-01 Holiday\n"
                         "2017-01-01 Holiday1\n"
                         "2017-01-01 Holiday2\n";
    EXPECT_NE(first.str(), second.c_str());
  }
  {
    std::string second = "2017-01-01 Holiday2\n"
                         "2017-01-01 Holiday1\n"
                         "2017-01-01 Holiday\n";
    EXPECT_NE(first.str(), second.c_str());
  }
}

TEST(DatabaseAddPrint, ErrorTest2) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2019-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2015-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }


  std::stringstream first;
  db.Print(first);
  std::string second = "2019-01-01 Holiday\n"
                       "2017-01-01 Holiday\n"
                       "2015-01-01 Holiday\n";
  EXPECT_NE(first.str(), second.c_str());
}

TEST(DatabaseAddPrint, ErrorTest3) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream first;
  db.Print(first);
  std::string second = "2017-01-01 Holiday\n"
                       "2017-01-01 Holiday\n"
                       "2017-01-01 Holiday\n";
  EXPECT_NE(first.str(), second.c_str());
}

TEST(DatabaseLast, SuccesTest1) {
  Database db;
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  {
    std::string first = db.Last(Date("2016-12-31"));
    std::string second = "No entries";
    EXPECT_EQ(first, second);
  }
  {
    std::string first = db.Last(Date("2017-01-01"));
    std::string second = "2017-01-01 Holiday";
    EXPECT_EQ(first, second);
  }
  {
    std::string first = db.Last(Date("2017-06-01"));
    std::string second = "2017-03-08 Holiday";
    EXPECT_EQ(first, second);
  }
}

TEST(DatabaseLast, SuccesTest2) {
  Database db;
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  {
    std::string first = db.Last(Date("2016-12-31"));
    std::string second = "No entries";
    EXPECT_EQ(first, second);
  }
}

TEST(DatabaseLast, SuccesTest3) {
  Database db;
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  {
    std::string first = db.Last(Date("2019-1-1"));
    std::string second = "2017-01-01 New Year";
    EXPECT_EQ(first, second);
  }
}

TEST(DatabaseLast, SuccesTest4) {
  Database db;
  {
    std::stringstream input("2017-01-01 Event1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2019-01-01 Event1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2020-01-01 Event1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  {
    std::string first = db.Last(Date("2018-1-1"));
    std::string second = "2017-01-01 Event1";
    EXPECT_EQ(first, second);
  }
}

TEST(DatabaseLast, SuccesTest5) {
  Database db;
  {
    std::stringstream input("2017-01-01 Event1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2019-01-01 Event1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2020-01-01 Event1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2022-01-01 Event1");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  {
    std::string first = db.Last(Date("2023-01-01"));
    std::string second = "2022-01-01 Event1";
    EXPECT_EQ(first, second);
  }
  {
    std::string first = db.Last(Date("2021-01-01"));
    std::string second = "2020-01-01 Event1";
    EXPECT_EQ(first, second);
  }
}

TEST(DatabaseFindIf, SuccesTest1) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("event != \"working day\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  const auto entries = db.FindIf(predicate);
  for (const auto& entry : entries) {
    first << entry << std::endl;
  }
  first << "Found " << entries.size() << " entries" << std::endl;

  std::string second = "2017-01-01 Holiday\n"
                       "2017-01-01 New Year\n"
                       "2017-03-08 Holiday\n"
                       "Found 3 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseFindIf, SuccesTest2) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("event != \"New Year\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  const auto entries = db.FindIf(predicate);
  for (const auto& entry : entries) {
    first << entry << std::endl;
  }
  first << "Found " << entries.size() << " entries" << std::endl;

  std::string second = "2017-01-01 Holiday\n"
                       "2017-03-08 Holiday\n"
                       "Found 2 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseFindIf, SuccesTest3) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("event != \"Friday\" AND date >= 2017-01-01 AND date < 2020-01-01");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  const auto entries = db.FindIf(predicate);
  for (const auto& entry : entries) {
    first << entry << std::endl;
  }
  first << "Found " << entries.size() << " entries" << std::endl;

  std::string second = "2017-01-01 Holiday\n"
                       "2017-01-01 New Year\n"
                       "2017-03-08 Holiday\n"
                       "Found 3 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseFindIf, SuccesTest4) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("event != \"Friday\" AND date > 2017-01-01 AND date < 2020-01-01");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  const auto entries = db.FindIf(predicate);
  for (const auto& entry : entries) {
    first << entry << std::endl;
  }
  first << "Found " << entries.size() << " entries" << std::endl;

  std::string second = "2017-03-08 Holiday\n"
                       "Found 1 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseFindIf, SuccesTest5) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  const auto entries = db.FindIf(predicate);
  for (const auto& entry : entries) {
    first << entry << std::endl;
  }
  first << "Found " << entries.size() << " entries" << std::endl;

  std::string second = "2017-01-01 Holiday\n"
                       "2017-01-01 New Year\n"
                       "2017-03-08 Holiday\n"
                       "Found 3 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseFindIf, SuccesTest6) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("date >= 2020-01-01");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  const auto entries = db.FindIf(predicate);
  for (const auto& entry : entries) {
    first << entry << std::endl;
  }
  first << "Found " << entries.size() << " entries" << std::endl;

  std::string second = "Found 0 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseFindIf, SuccesTest7) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("event == \"Holiday\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  const auto entries = db.FindIf(predicate);
  for (const auto& entry : entries) {
    first << entry << std::endl;
  }
  first << "Found " << entries.size() << " entries" << std::endl;

  std::string second = "2017-01-01 Holiday\n"
                       "2017-03-08 Holiday\n"
                       "Found 2 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseFindIf, SuccesTest8) {
  Database db;
  {
    std::stringstream input("2017-01-01 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-03-08 Holiday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-01-01 New Year");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("event == \"New Year\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  const auto entries = db.FindIf(predicate);
  for (const auto& entry : entries) {
    first << entry << std::endl;
  }
  first << "Found " << entries.size() << " entries" << std::endl;

  std::string second = "2017-01-01 New Year\n"
                       "Found 1 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseFindIf, SuccesTest9) {
  Database db;
  std::stringstream input("event == \"New Year\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  const auto entries = db.FindIf(predicate);
  for (const auto& entry : entries) {
    first << entry << std::endl;
  }
  first << "Found " << entries.size() << " entries" << std::endl;

  std::string second = "Found 0 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseRemoveIf, SuccesTest1) {
  Database db;
  {
    std::stringstream input("2017-06-01 1st of June");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 8th of July");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("date == 2017-07-08");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 2 entries\n"
                       "2017-06-01 1st of June\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseRemoveIf, SuccesTest2) {
  Database db;
  {
    std::stringstream input("2017-06-01 1st of June");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 8th of July");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 3 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseRemoveIf, SuccesTest3) {
  Database db;
  {
    std::stringstream input("2017-06-01 1st of June");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 8th of July");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("date >= 2017-06-01");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 3 entries\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseRemoveIf, SuccesTest4) {
  Database db;
  {
    std::stringstream input("2017-06-01 1st of June");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 8th of July");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("event == \"8th of July\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 1 entries\n"
                       "2017-06-01 1st of June\n"
                       "2017-07-08 Someone's birthday\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseRemoveIf, SuccesTest5) {
  Database db;
  {
    std::stringstream input("2017-06-01 1st of June");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 8th of July");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("event == \"Friday\"");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 0 entries\n"
                       "2017-06-01 1st of June\n"
                       "2017-07-08 8th of July\n"
                       "2017-07-08 Someone's birthday\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseRemoveIf, SuccesTest6) {
  Database db;
  {
    std::stringstream input("2017-06-01 1st of June");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 8th of July");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("date >= 2020-01-01");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 0 entries\n"
                       "2017-06-01 1st of June\n"
                       "2017-07-08 8th of July\n"
                       "2017-07-08 Someone's birthday\n";
  ASSERT_EQ(first.str(), second.c_str());
}

TEST(DatabaseRemoveIf, SuccesTest7) {
  Database db;
  {
    std::stringstream input("2017-06-01 1st of June");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 8th of July");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2020-01-01 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  {
    std::stringstream input("date <= 2020-01-01");
    std::stringstream first;
    auto condition = ParseCondition(input);
    auto predicate = [condition](const Date& date, const std::string& event) {
      return condition->Evaluate(date, event);
    };
    int count = db.RemoveIf(predicate);
    first << "Removed " << count << " entries" << std::endl;
    db.Print(first);
    std::string second = "Removed 4 entries\n";
    ASSERT_EQ(first.str(), second.c_str());
  }
}

TEST(DatabaseRemoveIf, SuccesTest8) {
  Database db;
  {
    std::stringstream input("2017-06-01 1st of June");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 8th of July");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2017-07-08 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2020-01-01 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }
  {
    std::stringstream input("2020-03-01 Someone's birthday");
    const auto date = ParseDate(input);
    const auto event = ParseEvent(input);
    db.Add(date, event);
  }

  std::stringstream input("date <= 2020-01-01");
  std::stringstream first;
  auto condition = ParseCondition(input);
  auto predicate = [condition](const Date& date, const std::string& event) {
    return condition->Evaluate(date, event);
  };
  int count = db.RemoveIf(predicate);
  first << "Removed " << count << " entries" << std::endl;
  db.Print(first);
  std::string second = "Removed 4 entries\n"
                       "2020-03-01 Someone's birthday\n";
  ASSERT_EQ(first.str(), second.c_str());
}
