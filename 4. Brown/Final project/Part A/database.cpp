//
// Created by ilya on 25.11.2019.
//

#include "database.h"

Stop::Stop() {
  name_ = "unnamed";
}

std::string Stop::GetName() const {
  return name_;
}

Coordinates Stop::GetCoord() const {
  return coord_;
}

std::string Route::GetName() const {
  return name_;
}

size_t LinearRoute::CountOfStops() const {
  return stops_.size() * 2 - 1;
}

size_t LinearRoute::CountOfUniqueStops() const {
  return stops_.size();
}

double LinearRoute::Length() const {
  double result = 0;
  for (size_t i = 0; i < stops_.size() - 2; ++i) {
    result += Coordinates::Distance(stops_[i]->GetCoord(), stops_[i + 1]->GetCoord()) * 2;
  }
  return result;
}

size_t CycleRoute::CountOfStops() const {
  return stops_.size() + 1;
}

size_t CycleRoute::CountOfUniqueStops() const {
  return stops_.size();
}

double CycleRoute::Length() const {
  double result = 0;
  for (size_t i = 0; i < stops_.size() - 2; ++i) {
    result += Coordinates::Distance(stops_[i]->GetCoord(), stops_[i + 1]->GetCoord());
  }
  result += Coordinates::Distance(stops_.back()->GetCoord(), stops_.front()->GetCoord());
  return result;
}

void Database::AddStop(Stop &&stop) {
  all_stops_.insert({stop.GetName(), std::make_shared<Stop>(stop)});
}

void Database::AddRoute(const std::string &route_name, std::vector<std::string> &&stop_names) {
  std::vector<std::shared_ptr<Stop>> stops_ptr;
  stops_ptr.reserve(stop_names.size());
  for(auto& str : stop_names) {
    auto res = all_stops_.insert({str, std::make_shared<Stop>()});
    stops_ptr.push_back(res.first->second);
  }
  all_routes_.insert({route_name, MakeRoute()});
}

std::shared_ptr<Route> Database::TakeRoute(std::string_view route_name) {

}