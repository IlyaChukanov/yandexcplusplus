//
// Created by ilya on 27.11.2019.
//
#include <gtest/gtest.h>
#include "database.h"

// TODO Тесты на stops
// TODO Тесты на routes

TEST(TestStop, Default) {
  std::string stop_name = "unnamed";
  Stop s;
  ASSERT_EQ(s.GetCoord().GetLatitude(), 0);
  ASSERT_EQ(s.GetCoord().GetLongitude(), 0);
  ASSERT_EQ(s.GetName(), stop_name);
}

TEST(TestStop, Test1) {
  std::string stop_name = "Stop1";
  Stop s(stop_name, Coordinates(1, 2));
  ASSERT_EQ(s.GetCoord().GetLatitude(), 1);
  ASSERT_EQ(s.GetCoord().GetLongitude(), 2);
  ASSERT_EQ(s.GetName(),stop_name);
}

TEST(TestStop, Test2) {
  std::string stop_name = "Stop2";
  Stop s(stop_name, Coordinates(3, 4));
  ASSERT_EQ(s.GetCoord().GetLatitude(), 3);
  ASSERT_EQ(s.GetCoord().GetLongitude(), 4);
  ASSERT_EQ(s.GetName(),stop_name);
}

TEST(TestStop, Test3) {
  std::string stop_name = "Stop3";
  Stop s(stop_name, Coordinates(23.3, 33.3));
  ASSERT_EQ(s.GetCoord().GetLatitude(), 23.3);
  ASSERT_EQ(s.GetCoord().GetLongitude(), 33.3);
  ASSERT_EQ(s.GetName(),stop_name);
}

TEST(TestCtor, TestCopy) {
  std::string stop_name = "Stop1";
  Stop s1(stop_name, Coordinates(23.3, 33.3));
  Stop s2(s1);
  ASSERT_EQ(s2.GetCoord().GetLatitude(), 23.3);
  ASSERT_EQ(s2.GetCoord().GetLongitude(), 33.3);
  ASSERT_EQ(s2.GetName(),stop_name);
}

TEST(TestStopCpyCtor, TestMove) {
  std::string stop_name = "Stop1";
  Stop s1(stop_name, Coordinates(23.3, 33.3));
  Stop s2(std::move(s1));
  ASSERT_EQ(s2.GetCoord().GetLatitude(), 23.3);
  ASSERT_EQ(s2.GetCoord().GetLongitude(), 33.3);
  ASSERT_EQ(s2.GetName(),stop_name);
}

TEST(TestStopAssign, TestCopy) {
  std::string stop_name = "Stop1";
  Stop s1(stop_name, Coordinates(23.3, 33.3));
  Stop s2;
  s2 = s1;
  ASSERT_EQ(s2.GetCoord().GetLatitude(), 23.3);
  ASSERT_EQ(s2.GetCoord().GetLongitude(), 33.3);
  ASSERT_EQ(s2.GetName(),stop_name);
}

TEST(TestStopAssign, TestMove) {
  std::string stop_name = "Stop1";
  Stop s1(stop_name, Coordinates(23.3, 33.3));
  Stop s2;
  s2 = std::move(s1);
  ASSERT_EQ(s2.GetCoord().GetLatitude(), 23.3);
  ASSERT_EQ(s2.GetCoord().GetLongitude(), 33.3);
  ASSERT_EQ(s2.GetName(),stop_name);
}

TEST(TestNotAddedStop, Test1) {
  Database db;
  ASSERT_EQ(db.TakeStop("Stop1"), nullptr);
}

TEST(TestNotAddedStop, Test2) {
  Database db;
  ASSERT_EQ(db.TakeStop("Stop1"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop2"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop3"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop4"), nullptr);
}

TEST(TestNotAddedStop, Test3) {
  Database db;
  ASSERT_EQ(db.TakeStop("Stop1"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop2"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop3"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop4"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop4"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop3"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop2"), nullptr);
  ASSERT_EQ(db.TakeStop("Stop1"), nullptr);
}

TEST(TestAddStop, Base1) {
  Database db;
  db.AddStop({"Stop1", Coordinates(1, 2)});
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop1")->GetCoord().GetLatitude(), 1);
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop1")->GetCoord().GetLongitude(), 2);
}

TEST(TestAddStop, Base2) {
  Database db;
  db.AddStop({"Stop1", Coordinates(1, 2)});
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop1")->GetCoord().GetLatitude(), 1);
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop1")->GetCoord().GetLongitude(), 2);
  db.AddStop({"Stop2", Coordinates(3, 4)});
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop2")->GetCoord().GetLatitude(), 3);
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop2")->GetCoord().GetLongitude(), 4);
}

TEST(TestAddStop, ManyAdd) {
  Database db;
  db.AddStop({"Stop1", Coordinates(1, 2)});
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop1")->GetCoord().GetLatitude(), 1);
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop1")->GetCoord().GetLongitude(), 2);
  db.AddStop({"Stop2", Coordinates(3, 4)});
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop2")->GetCoord().GetLatitude(), 3);
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop2")->GetCoord().GetLongitude(), 4);
  db.AddStop({"Stop3", Coordinates(5, 6)});
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop3")->GetCoord().GetLatitude(), 5);
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop3")->GetCoord().GetLongitude(), 6);
  db.AddStop({"Stop4", Coordinates(7.7, 8.8)});
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop4")->GetCoord().GetLatitude(), 7.7);
  ASSERT_DOUBLE_EQ(db.TakeStop("Stop4")->GetCoord().GetLongitude(), 8.8);
}

TEST(TestAddRoute, AddCycle) {
  Database db;
  std::string route_name = "Route1";
  std::vector<std::string> names{"Stop1", "Stop2", "Stop3", "Stop4"};
  std::vector<std::shared_ptr<Stop>> stops_;
  for (const auto& name : names) {
    stops_.push_back(db.TakeStop(name));
  }
  db.AddRoute(route_name, std::make_shared<CycleRoute>(route_name, names, stops_));
  auto route = db.TakeRoute(route_name);
  ASSERT_EQ(route->GetName(), route_name);
  auto take_names = route->GetStopsName();
  ASSERT_EQ(names.size(), take_names.size());
  for (size_t i = 0; i < names.size(); ++i) {
    ASSERT_EQ(names[i], take_names[i]);
    ASSERT_EQ(db.TakeStop(names[i]), nullptr);
  }
  // Добавить тест на длину пути
}

TEST(TestRouteBuilder, AddCycle) {
  
}