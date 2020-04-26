//
// Created by ilya on 25.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_REQUEST_H
#define YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_REQUEST_H
#include <string>
#include <optional>

#include "json.h"
#include "database.h"
#include "route.h"
#include "map.h"

namespace TransportDatabase {

std::vector<std::string> SplitBy(std::string_view s, char split = '.');
std::string Strip(std::string_view str);

class Request;
using RequestHolder = std::unique_ptr<Request>;
class Request {
public:
  enum class Type {
    ADD_ROUTE,
    ADD_STOP,
    TAKE_ROUTE,
    TAKE_STOP,
    CREATE_ROUTE,
    CREATE_MAP
  };
  explicit Request(Type type) : type_(type) {}
  virtual void ParseFromJSON(const Json::Node& node) = 0;
  virtual ~Request() = default;

  static RequestHolder Create(Request::Type type);

  Type GetType() const {
    return type_;
  }
private:
  Type type_;
};

template <typename Result, typename Processor>
class ReadRequest : public Request {
public:
  using Request::Request;
  void ParseFromJSON(const Json::Node& node) override = 0;
  virtual Result Process(Processor &db) const = 0;
  virtual Json::Node JSONAnswer(const Result& result) const = 0;
protected:
  int request_id = 0;
};

template <typename Processor>
class ModifyRequest : public Request {
public:
  using Request::Request;
  virtual void Process(Processor& db) const = 0;
};

class AddStopRequest : public ModifyRequest<Database> {
public:
  AddStopRequest() : ModifyRequest(Request::Type::ADD_STOP) {}
  void ParseFromJSON(const Json::Node& node) override;
  void Process(Database& db) const override;
private:
  std::string stop_name;
  double latitude, longitude;
  std::vector<std::pair<std::string, int>> distances;
};

class AddRouteRequest : public ModifyRequest<Database> {
public:
  AddRouteRequest() : ModifyRequest(Request::Type::ADD_ROUTE) {}
  void ParseFromJSON(const Json::Node& node) override;
  void Process(Database& db) const override;
private:
  std::string route_name;
  Route::RouteTypes route_type;
  std::vector<std::string> stops_name;
};

struct TakeRouteAnswer {
  int id;
  bool has_value;
  std::string route_name;
  size_t stops_count;
  size_t unique_stops_count;
  double length;
  double curvature;
};

class TakeRouteRequest : public ReadRequest<TakeRouteAnswer, Database> {
public:
  TakeRouteRequest() : ReadRequest(Request::Type::TAKE_ROUTE) {}
  void ParseFromJSON(const Json::Node& node) override;
  TakeRouteAnswer Process(Database& db) const override;
  Json::Node JSONAnswer(const TakeRouteAnswer& result) const override;
private:
  std::string route_name;
};

struct TakeStopAnswer {
  int id;
  bool in_base;
  std::string stop_name;
  std::vector<std::string> names;
};

class TakeStopRequest : public ReadRequest<TakeStopAnswer, Database> {
public:
  TakeStopRequest() : ReadRequest(Request::Type::TAKE_STOP) {}
  void ParseFromJSON(const Json::Node& node) override;
  TakeStopAnswer Process(Database& db) const override;
  Json::Node JSONAnswer(const TakeStopAnswer& result) const override;
private:
  std::string stop_name;
};

struct CreateRouteAnswer {
  int id;
  bool has_route;
  double total_time;
  std::list<std::unique_ptr<BaseNode>> nodes;
};

class CreateRouteRequest : public ReadRequest<CreateRouteAnswer, Router> {
public:
  CreateRouteRequest() : ReadRequest(Request::Type::CREATE_ROUTE) {}
  void ParseFromJSON(const Json::Node& node) override;
  CreateRouteAnswer Process(Router& db) const override;
  Json::Node JSONAnswer(const CreateRouteAnswer& result) const override;
private:
  std::string from;
  std::string to;
};

struct CreateMapAnswer {
  int id;
  std::string svg;
};

class CreateMapRequest : public ReadRequest<CreateMapAnswer, Map> {
public:
  CreateMapRequest() : ReadRequest(Request::Type::CREATE_MAP) {}
  void ParseFromJSON(const Json::Node& node) override;
  CreateMapAnswer Process(Map& db) const override;
  Json::Node JSONAnswer(const CreateMapAnswer& result) const override;
};
}
#endif //YANDEXYELLOWFINAL_4_BROWN_FINAL_PROJECT_PART_A_REQUEST_H
