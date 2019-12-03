//
// Created by ilya on 25.11.2019.
//
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include "request.h"

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
  default:
    return nullptr;
  }
}

void AddStopRequest::ParseFrom(std::string_view input) {
  size_t delim_name = input.find(':');
  stop_name = Strip(input.substr(0, delim_name));
  auto tokens = SplitBy(input.substr(delim_name + 2), ',');
  latitude = std::stod(Strip(tokens[0]));
  longitude = std::stod(Strip(tokens[1]));
  for (size_t i = 2; i < tokens.size(); ++i) {
    size_t m_pos = tokens[i].find('m');
    double distance = std::stod(Strip(tokens[i].substr(0, m_pos)));
    auto tail = tokens[i].substr(m_pos);
    size_t to_pos = tail.find('o');
    std::string name = Strip(tail.substr(to_pos + 1));
    distances.emplace_back(name, distance);
  }
}

void AddStopRequest::ParseFromJSON(const Json::Node &node) {
  auto map_elem = node.AsMap();
  stop_name = map_elem["name"].AsString();
  latitude = map_elem["latitude"].AsDouble();
  longitude = map_elem["longitude"].AsDouble();
  auto distance_map = map_elem["road_distances"].AsMap();
  for (const auto& [key, value] : distance_map) {
    distances.emplace_back(key, value.AsInt64());
  }
}

void AddStopRequest::Process(Database &db) const {
  db.AddStop({stop_name,
             CoordinatesBuilder().SetLatitude(latitude).SetLongitude(longitude).Build(), distances});
}


void AddRouteRequest::ParseFrom(std::string_view input) {
  size_t delim_name = input.find(':');
  route_name = Strip(input.substr(0, delim_name));
  std::vector<char> types = {'-', '>'};
  char delim_type = '-';
  for (auto type : types) {
    size_t delim_pos = input.find(type);
    if (delim_pos != input.npos) {
      delim_type = type;
      route_type = sign_to_route.at(delim_type);
      break;
    }
  }
  auto tokens = SplitBy(input.substr(delim_name + 2), delim_type);
  stops_name.reserve(tokens.size());
  for (auto& token : tokens) {
    std::string stop;
    if (token.front() == ' ') {
      if (token.back() == ' ') {
        stop = Strip(token.substr(1, token.size() - 2));
      }
      else {
        stop = Strip(token.substr(1,token.size() - 1));
      }
    }
    else {
      stop = Strip(token.substr(0,token.size() - 1));
    }
    stops_name.push_back(stop);
  }
}

void AddRouteRequest::ParseFromJSON(const Json::Node &node) {
  auto map_elem = node.AsMap();
  route_name = map_elem["name"].AsString();
  route_type = map_elem["is_roundtrip"].AsBool() ? Route::RouteTypes::CYCLE : Route::RouteTypes::LINEAR;
  auto stops = map_elem["stops"].AsArray();
  for (const auto& stop : stops) {
    stops_name.push_back(stop.AsString());
  }
}

void AddRouteRequest::Process(Database &db) const {
  db.AddRoute(route_name,
              RouteBuilder(db).MakeRoute({route_type, route_name, stops_name}));
}

void TakeRouteRequest::ParseFrom(std::string_view input) {
  route_name = input;
}

void TakeRouteRequest::ParseFromJSON(const Json::Node &node) {
  auto map_elem = node.AsMap();
  route_name = map_elem["name"].AsString();
  request_id = map_elem["id"].AsInt64();
}

TakeRouteAnswer TakeRouteRequest::Process(const Database &db) const {
  auto route = db.TakeRoute(route_name);
  if (route) {
    return {request_id, true, route_name, route->CountOfStops(), route->CountOfUniqueStops(), route->RealLength(), route->Curvature()};
  }
  else {
    return {request_id, false, route_name, 0, 0, 0};
  }
}

std::string TakeRouteRequest::StringAnswer(const TakeRouteAnswer &result) const {
  std::stringstream s;
  if (result.has_value) {
    s << std::setprecision(6) << "Bus " << result.route_name << ": " << result.stops_count << " stops on route, " <<
      result.unique_stops_count << " unique stops, " << result.length << " route length, " << result.curvature << " curvature";
  }
  else {
    s << "Bus " << result.route_name << ": not found";
  }
  return s.str();
}

std::string TakeRouteRequest::JSONAnswer(const TakeRouteAnswer &result) const {
  std::stringstream s;
  if (result.has_value) {
    s << "{";
    s << R"("request_id": )" << result.id << ", ";
    s << R"("stop_count": )" << result.stops_count << ", ";
    s << R"("unique_stop_count": )" << result.unique_stops_count  << ", ";
    s << R"("route_length": )" << std::setprecision(6) << result.length << ", ";
    s << R"("curvature": )" << std::setprecision(6) << result.curvature;
    s << "}";
  }
  else {
    s << "{";
    s << R"("request_id": )" << result.id << ", ";
    s << R"("error_message": "not found")";
    s << "}";
  }
  return s.str();
}

void TakeStopRequest::ParseFrom(std::string_view input) {
  stop_name = input;
}

void TakeStopRequest::ParseFromJSON(const Json::Node &node) {
  auto map_elem = node.AsMap();
  stop_name = map_elem["name"].AsString();
  request_id = map_elem["id"].AsInt64();
}

TakeStopAnswer TakeStopRequest::Process(const Database &db) const {
  auto stop = db.TakeStop(stop_name);
  if (stop) {
    return {request_id, true, stop_name, stop->TakeRoutes()};
  }
  else {
    return {request_id, false, stop_name, {}};
  }
}

std::string TakeStopRequest::StringAnswer(const TakeStopAnswer &result) const {
  std::stringstream s;
  if (result.in_base) {
    if (result.names.empty()) {
      s << "Stop " << result.stop_name << ": no buses";
    }
    else {
      s << "Stop " << result.stop_name << ": buses";
      for (const auto& route : result.names) {
        s << " " << route;
      }
    }
  }
  else {
    s << "Stop " << result.stop_name << ": not found";
  }
  return s.str();
}

std::string TakeStopRequest::JSONAnswer(const TakeStopAnswer &result) const {
  std::stringstream s;
  if (result.in_base) {
    if (result.names.empty()) {
      s << "{";
      s << R"("request_id": )" << result.id << ", ";
      s << R"("buses": [])";
      s << "}";
    }
    else {
      s << "{";
      s << R"("request_id": )" << result.id << ", ";
      s << R"("buses": [)";
      for (size_t i = 0; i < result.names.size(); ++i) {
        if (!i) {
          s << "\"" << result.names[i] << "\"";
        }
        else {
          s << ", " << "\"" << result.names[i] << "\"";
        }
      }
      s << "]";
      s << "}";
    }
  }
  else {
    s << "{";
    s << R"("request_id": )" << result.id << ", ";
    s << R"("error_message": "not found")";
    s << "}";
  }
  return s.str();
}