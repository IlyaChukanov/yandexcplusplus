//
// Created by ilya on 28.11.2019.
//
#include <gtest/gtest.h>
#include "request.h"

TEST(TestAddRequest, TestStopReqest1) {
  auto req = std::make_unique<AddStopRequest>();
  ASSERT_EQ(req->GetType(), Request::Type::ADD_STOP);
  std::string request{"Stop1: 1, 2"};
  req->ParseFrom(request);
  Database db;
  req->Process(db);
  auto stop = db.TakeStop("Stop1");
  ASSERT_EQ(stop->GetName(), "Stop1");
  ASSERT_DOUBLE_EQ(stop->GetCoord().GetLatitude(), 1);
  ASSERT_DOUBLE_EQ(stop->GetCoord().GetLongitude(), 2);
}

TEST(TestAddRequest, TestStopReqest2) {
  auto req = std::make_unique<AddStopRequest>();
  ASSERT_EQ(req->GetType(), Request::Type::ADD_STOP);
  std::string request{"Stop2: 22.22, 33.33"};
  req->ParseFrom(request);
  Database db;
  req->Process(db);
  auto stop = db.TakeStop("Stop2");
  ASSERT_EQ(stop->GetName(), "Stop2");
  ASSERT_DOUBLE_EQ(stop->GetCoord().GetLatitude(), 22.22);
  ASSERT_DOUBLE_EQ(stop->GetCoord().GetLongitude(), 33.33);
}

TEST(TestAddRequest, TestRouteReqest1) {
  auto req = std::make_unique<AddRouteRequest>();
  ASSERT_EQ(req->GetType(), Request::Type::ADD_ROUTE);
  std::string request{"Bus1: Stop1 - Stop2 - Stop3 - Stop4"};
  std::vector<std::string> stops_name = {"Stop1", "Stop2", "Stop3", "Stop4"};
  req->ParseFrom(request);
  Database db;
  req->Process(db);
  auto route = db.TakeRoute("Bus1");
  ASSERT_EQ(route->GetName(), "Bus1");
  ASSERT_EQ(route->GetStopsName(), stops_name);
  ASSERT_EQ(route->CountOfStops(), 7);
}

TEST(TestAddRequest, TestRouteReqest2) {
  auto req = std::make_unique<AddRouteRequest>();
  ASSERT_EQ(req->GetType(), Request::Type::ADD_ROUTE);
  std::string request{"Bus1: Stop1 > Stop2 > Stop3 > Stop4"};
  std::vector<std::string> stops_name = {"Stop1", "Stop2", "Stop3", "Stop4"};
  req->ParseFrom(request);
  Database db;
  req->Process(db);
  auto route = db.TakeRoute("Bus1");
  ASSERT_EQ(route->GetName(), "Bus1");
  ASSERT_EQ(route->GetStopsName(), stops_name);
  ASSERT_EQ(route->CountOfStops(), 5);
}