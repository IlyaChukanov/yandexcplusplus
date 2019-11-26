//
// Created by ilya on 25.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_DATABASE_H
#define YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_DATABASE_H
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "coordinates.h"

class Stop {
public:

  Stop();
  Stop(const std::string& name, const Coordinates& coord) : name_(name), coord_(coord) {}
  Stop(const Stop& other) = default;
  Stop(Stop&& other) = default;
  Stop& operator=(const Stop& other) = default;
  Stop& operator=(Stop&& other) = default;

  std::string GetName() const;
  Coordinates GetCoord() const;

private:
  std::string name_;
  Coordinates coord_;
};

class Route {
public:
  enum class RouteTypes {LINEAR, CYCLE};
  explicit Route(const std::string& name) : name_(name) {}
  virtual ~Route() = 0;
  virtual size_t CountOfStops() const = 0;
  virtual size_t CountOfUniqueStops() const = 0;
  virtual double Length() const = 0;
  std::string GetName() const;
private:
  std::string name_;
};

class LinearRoute : public Route {
public:
  LinearRoute() = default;
  explicit LinearRoute(const std::string& name, std::vector<std::shared_ptr<Stop>>&& stops) : Route(name), stops_(stops) {}
  size_t CountOfStops() const override;
  size_t CountOfUniqueStops() const override;
  double Length() const override;
private:
  std::vector<std::shared_ptr<Stop>> stops_;
};

class CycleRoute : public Route {
public:
  CycleRoute() = default;
  explicit CycleRoute(const std::string& name, std::vector<std::shared_ptr<Stop>>&& stops) : Route(name), stops_(stops) {}
  size_t CountOfStops() const override;
  size_t CountOfUniqueStops() const override;
  double Length() const override;
private:
  std::vector<std::shared_ptr<Stop>> stops_;
};

std::shared_ptr<Route> MakeRoute(Route::RouteTypes type, std::string&& name, std::vector<std::shared_ptr<Stop>>&& stops) {
  switch (type) {
  case Route::RouteTypes::LINEAR:
    return std::make_shared<LinearRoute>(stops);
  case Route::RouteTypes::CYCLE:
    return std::make_shared<CycleRoute>(stops);
  default:
    return nullptr;
  }
}

class Database {
  using StopData = std::unordered_map<std::string, std::shared_ptr<Stop>>;
  using RouteData = std::unordered_map<std::string, std::shared_ptr<Route>>;
public:
  Database() = default;
  void AddStop(Stop&& stop);
  void AddRoute(const std::string& route_name, std::vector<std::string>&& stop_names);
  std::shared_ptr<Route> TakeRoute(std::string_view route_name);
private:
  StopData all_stops_;
  RouteData all_routes_;
};

#endif //YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_DATABASE_H
