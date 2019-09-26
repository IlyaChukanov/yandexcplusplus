#include <gtest/gtest.h>

#include <sstream>
#include <memory>

#include "condition_parser.h"

// Тесты на разбор выражений так же включают в себя тесты на

TEST(ParseCondionTest, Test1) {
  std::istringstream is("date != 2017-11-18");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_TRUE(root->Evaluate({2017, 1, 1}, ""));
  EXPECT_FALSE(!root->Evaluate({2017, 1, 1}, ""));
}

TEST(ParseCondionTest, Test2) {
  std::istringstream is(R"(event == "sport event")");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_TRUE(root->Evaluate({2017, 1, 1}, "sport event"));
  EXPECT_FALSE(root->Evaluate({2017, 1, 1}, "holiday"));
}

TEST(ParseCondionTest, Test3) {
  std::istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_TRUE(root->Evaluate({2017, 1, 1}, ""));
  EXPECT_TRUE(root->Evaluate({2017, 3, 1}, ""));
  EXPECT_TRUE(root->Evaluate({2017, 6, 30}, ""));
  EXPECT_FALSE(root->Evaluate({2017, 7, 1}, ""));
  EXPECT_FALSE(root->Evaluate({2016, 12, 31}, ""));
}

TEST(ParseCondionTest, Test4) {
  std::istringstream is(R"(event != "sport event" AND event != "Wednesday")");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_TRUE(root->Evaluate({2017, 1, 1}, "holiday"));
  EXPECT_FALSE(root->Evaluate({2017, 1, 1}, "sport event"));
  EXPECT_FALSE(root->Evaluate({2017, 1, 1}, "Wednesday"));
}

TEST(ParseCondionTest, Test5) {
  std::istringstream is(R"(event != "sport event" AND event != "Wednesday")");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_TRUE(root->Evaluate({2017, 1, 1}, "holiday"));
  EXPECT_FALSE(root->Evaluate({2017, 1, 1}, "sport event"));
  EXPECT_FALSE(root->Evaluate({2017, 1, 1}, "Wednesday"));
}

TEST(ParseCondionTest, Test6) {
  std::istringstream is(R"(event == "holiday AND date == 2017-11-18")");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_FALSE(root->Evaluate({2017, 11, 18}, "holiday"));
  EXPECT_FALSE(root->Evaluate({2017, 11, 18}, "work day"));
  EXPECT_TRUE(root->Evaluate({1, 1, 1}, "holiday AND date == 2017-11-18"));
}

TEST(ParseCondionTest, Test7) {
  std::istringstream is(R"(((event == "holiday" AND date == 2017-01-01)))");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_TRUE(root->Evaluate({2017, 1, 1}, "holiday"));
  EXPECT_FALSE(root->Evaluate({2017, 1, 2}, "holiday"));
}

TEST(ParseCondionTest, Test8) {
  std::istringstream is(R"(date > 2017-01-01 AND (event == "holiday" OR date < 2017-07-01))");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_FALSE(root->Evaluate({2016, 1, 1}, "holiday"));
  EXPECT_TRUE(root->Evaluate({2017, 1, 2}, "holiday"));
  EXPECT_TRUE(root->Evaluate({2017, 1, 2}, "workday"));
  EXPECT_FALSE(root->Evaluate({2018, 1, 2}, "workday"));
}

TEST(ParseCondionTest, Test9) {
  std::istringstream is(R"(date > 2017-01-01 AND event == "holiday" OR date < 2017-07-01)");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_TRUE(root->Evaluate({2016, 1, 1}, "event"));
  EXPECT_TRUE(root->Evaluate({2017, 1, 2}, "holiday"));
  EXPECT_TRUE(root->Evaluate({2017, 1, 2}, "workday"));
  EXPECT_FALSE(root->Evaluate({2018, 1, 2}, "workday"));
}

TEST(ParseCondionTest, Test10) {
  std::istringstream is(R"(((date == 2017-01-01 AND event == "holiday")))");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_TRUE(root->Evaluate({2017, 1, 1}, "holiday"));
  EXPECT_FALSE(root->Evaluate({2017, 1, 2}, "holiday"));
}

TEST(ParseCondionTest, Test11) {
  std::istringstream is(R"(((event == "2017-01-01" OR date > 2016-01-01)))");
  std::shared_ptr<Node> root = ParseCondition(is);
  EXPECT_TRUE(root->Evaluate({1, 1, 1}, "2017-01-01"));
  EXPECT_FALSE(root->Evaluate({2016, 1, 1}, "event"));
  EXPECT_TRUE(root->Evaluate({2016, 1, 2}, "event"));
}
