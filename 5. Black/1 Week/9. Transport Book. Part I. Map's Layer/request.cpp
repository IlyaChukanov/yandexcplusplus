//
// Created by ilya on 25.11.2019.
//
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <map>
#include <cassert>
#include <fstream>
#include "json.h"
#include "request.h"

namespace TransportDatabase {
std::vector<std::string> SplitBy(std::string_view s, char split) {
  std::vector<std::string> result;
  auto curr = 0;
  while (curr != s.npos) {
    auto space = s.find(split, curr);
    result.emplace_back(s.substr(curr, space - curr));
    if (space != s.npos) curr = (space + 1);
    else curr = s.npos;
  }
  return result;
}


std::string Strip(std::string_view str) {
  size_t left_border = 0;
  while (str[left_border] == ' ') {
    left_border++;
  }
  size_t right_border = str.size() - 1;
  while (str[right_border] == ' ') {
    right_border--;
  }
  return std::string(str.substr(left_border, right_border - left_border + 1));
}

RequestHolder Request::Create(Request::Type type) {
  switch (type) {
  case Request::Type::ADD_ROUTE:
    return std::make_unique<AddRouteRequest>();
  case Request::Type::ADD_STOP:
    return std::make_unique<AddStopRequest>();
  case Request::Type::TAKE_ROUTE:
    return std::make_unique<TakeRouteRequest>();
  case Request::Type::TAKE_STOP:
    return std::make_unique<TakeStopRequest>();
  case Request::Type::CREATE_ROUTE:
    return std::make_unique<CreateRouteRequest>();
  case Request::Type::CREATE_MAP:
    return std::make_unique<CreateMapRequest>();
  default:
    return nullptr;
  }
}

void AddStopRequest::ParseFromJSON(const Json::Node &node) {
  const auto& map_elem = node.AsMap();
  stop_name = map_elem.at("name").AsString();
  latitude = map_elem.at("latitude").AsDouble();
  longitude = map_elem.at("longitude").AsDouble();
  if (map_elem.count("road_distances")) {
    const auto& distance_map = map_elem.at("road_distances").AsMap();
    for (const auto& [key, value] : distance_map) {
      std::string name = key;
      int distance = value.AsInt();
      distances.emplace_back(name, distance);
    }
  }
}

void AddStopRequest::Process(Database &db ) const {
  db.AddStop({stop_name,
             CoordinatesBuilder().SetLatitude(latitude).SetLongitude(longitude).Build(), distances});
}

void AddRouteRequest::ParseFromJSON(const Json::Node &node) {
  const auto& map_elem = node.AsMap();
  route_name = map_elem.at("name").AsString();
  route_type = map_elem.at("is_roundtrip").AsBool() ? Route::RouteTypes::CYCLE : Route::RouteTypes::LINEAR;
  if (map_elem.count("stops")) {
    const auto& stops = map_elem.at("stops").AsArray();
    for (const auto& stop : stops) {
      stops_name.push_back(stop.AsString());
    }
  }
}

void AddRouteRequest::Process(Database &db) const {
  db.AddRoute(route_name,
              RouteBuilder(db).MakeRoute({route_type, route_name, stops_name}));
}

void TakeRouteRequest::ParseFromJSON(const Json::Node &node) {
  const auto& map_elem = node.AsMap();
  route_name = map_elem.at("name").AsString();
  request_id = map_elem.at("id").AsInt();
}

TakeRouteAnswer TakeRouteRequest::Process(Database &db) const {
  auto route = db.TakeRoute(route_name);
  if (route) {
    return {request_id, true, route_name, route->CountOfStops(), route->CountOfUniqueStops(), route->RealLength(), route->Curvature()};
  }
  else {
    return {request_id, false, route_name, 0, 0, 0, 0};
  }
}

Json::Node TakeRouteRequest::JSONAnswer(const TakeRouteAnswer &result) const {
  std::map<std::string, Json::Node> answer;
  answer["request_id"] = Json::Node(result.id);
  if (result.has_value) {
    answer["stop_count"] = Json::Node(static_cast<int>(result.stops_count));
    answer["unique_stop_count"] = Json::Node(static_cast<int>(result.unique_stops_count));
    answer["route_length"] = Json::Node(result.length);
    answer["curvature"] = Json::Node(result.curvature);
  }
  else {
    answer["error_message"] = Json::Node(std::string("not found"));
  }
  return Json::Node(answer);
}

void TakeStopRequest::ParseFromJSON(const Json::Node &node) {
  const auto& map_elem = node.AsMap();
  stop_name = map_elem.at("name").AsString();
  request_id = map_elem.at("id").AsInt();
}

TakeStopAnswer TakeStopRequest::Process(Database &db) const {
  auto stop = db.TakeStop(stop_name);
  if (stop) {
    return {request_id, true, stop_name, stop->TakeRoutes()};
  }
  else {
    return {request_id, false, stop_name, {}};
  }
}

Json::Node TakeStopRequest::JSONAnswer(const TakeStopAnswer &result) const {
  std::map<std::string, Json::Node> answer;
  answer["request_id"] = Json::Node(result.id);
  if (result.in_base) {
    if (result.names.empty()) {
      answer["buses"] = Json::Node(std::vector<Json::Node>());
    }
    else {
      std::vector<Json::Node> bufer;
      bufer.resize(result.names.size());
      for (size_t i = 0; i < result.names.size(); ++i) {
        bufer[i] = Json::Node(std::string(result.names[i]));
      }
      answer["buses"] = std::move(Json::Node(bufer));
    }
  }
  else {
    answer["error_message"] = Json::Node(std::string("not found"));
  }
  return Json::Node(answer);
}

void CreateRouteRequest::ParseFromJSON(const Json::Node &node) {
  const auto& map_elem = node.AsMap();
  from = map_elem.at("from").AsString();
  to = map_elem.at("to").AsString();
  request_id = map_elem.at("id").AsInt();
}

CreateRouteAnswer CreateRouteRequest::Process(Router& db) const {
  auto nodes = db.CreateRoute(from, to);
  auto first = dynamic_cast<InfoNode&>(*nodes.front());
  nodes.pop_front();
  if (!first.total_time) {
    return {request_id, false, {}};
  }
  return {request_id, true, first.total_time.value(), std::move(nodes)};
}

Json::Node CreateRouteRequest::JSONAnswer(const CreateRouteAnswer &result) const {
  std::map<std::string, Json::Node> answer;
  answer["request_id"] = Json::Node(result.id);
  if (!result.has_route) {
    answer["error_message"] = Json::Node(std::string("not found"));
    return answer;
  }
  answer["total_time"] = Json::Node(result.total_time);
  std::vector<Json::Node> items;
  for (const auto& node : result.nodes) {
    if (node->type == NodeType::WAIT) {
      auto node_wait = dynamic_cast<WaitNode&>(*node);
      std::map<std::string, Json::Node> wait_ans;
      wait_ans["type"] = Json::Node(std::string("Wait"));
      wait_ans["stop_name"] = Json::Node(node_wait.stop_name);
      wait_ans["time"] = Json::Node(node_wait.time);
      items.emplace_back(wait_ans);
    }
    if (node->type == NodeType::BUS) {
      auto node_bus = dynamic_cast<BusNode&>(*node);
      std::map<std::string, Json::Node> bus_ans;
      bus_ans["type"] = Json::Node(std::string("Bus"));
      bus_ans["bus"] = Json::Node(node_bus.route_name);
      bus_ans["span_count"] = Json::Node(node_bus.span_count);
      bus_ans["time"] = Json::Node(node_bus.time);
      items.emplace_back(bus_ans);
    }
  }
  answer["items"] = Json::Node(items);
  return answer;
}

void CreateMapRequest::ParseFromJSON(const Json::Node &node) {
  const auto& map_elem = node.AsMap();
  request_id = map_elem.at("id").AsInt();
}

CreateMapAnswer CreateMapRequest::Process(TransportDatabase::Map &db) const {
  db.CreateMap();
  auto res = db.RenderMap();
  return {request_id, res};
}

Json::Node CreateMapRequest::JSONAnswer(const TransportDatabase::CreateMapAnswer &result) const {
  std::map<std::string, Json::Node> answer;
  answer["request_id"] = Json::Node(result.id);
  answer["map"] = result.svg;
  std::ofstream dbg("res.svg");
  dbg << result.svg;
  return answer;
}

}