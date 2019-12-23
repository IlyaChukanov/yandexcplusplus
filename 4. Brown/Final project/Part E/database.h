//
// Created by ilya on 25.11.2019.
//
#ifndef YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_DATABASE_H
#define YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_DATABASE_H
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>
#include <list>

#include "graph.h"
#include "router.h"
#include "json.h"
#include "coordinates.h"

class Stop {
public:

  Stop();
  Stop(std::string  name, const Coordinates& coord);
  Stop(std::string  name, const Coordinates& coord, const std::vector<std::pair<std::string, int>>& distances);
  Stop(const Stop& other);
  Stop(Stop&& other) noexcept;
  Stop& operator=(const Stop& other);
  Stop& operator=(Stop&& other) noexcept;

  std::string GetName() const;
  Coordinates GetCoord() const;

  void AddRoute(const std::string& route_name);
  std::vector<std::string> TakeRoutes() const;

  std::unordered_map<std::string, int> distance_to_stop;
private:
  std::string name_;
  Coordinates coord_;
  std::set<std::string> routes_for_stop;
};

class Route {
public:
  enum class RouteTypes {LINEAR, CYCLE};
  explicit Route(const std::string& name, const std::vector<std::string>& stops_name, RouteTypes type) : name_(name), stops_name_(stops_name), route_type(type) {}
  virtual ~Route() = default;
  virtual size_t CountOfStops() const = 0;
  virtual size_t CountOfUniqueStops() const = 0;
  virtual double RealLength() const = 0;
  virtual double Length() const = 0;
  double Curvature() const;

  std::string GetName() const;
  std::vector<std::string> GetStopsName() const;

  RouteTypes route_type;
protected:
  std::string name_;
  std::vector<std::string> stops_name_;
};

static const std::unordered_map<char, Route::RouteTypes> sign_to_route = {{'-', Route::RouteTypes::LINEAR},
                                                                          {'>', Route::RouteTypes::CYCLE}};

class LinearRoute : public Route {
public:
  LinearRoute() = default;
  explicit LinearRoute(const std::string& name, const std::vector<std::string>& stops_name,
                       const std::vector<std::shared_ptr<Stop>>& stops) : Route(name, stops_name, RouteTypes::LINEAR), stops_(stops) {
    for (const auto& elem : stops_name_) {
      unique_stops_.insert(elem);
    }
  }
  size_t CountOfStops() const override;
  size_t CountOfUniqueStops() const override;
  double RealLength() const override;
  double Length() const override;
private:
  // Порядок маршрутов с данными о остановках
  std::vector<std::shared_ptr<Stop>> stops_;
  // Хранение уникальных остановок
  std::unordered_set<std::string_view> unique_stops_;
};

class CycleRoute : public Route {
public:
  CycleRoute() = default;
  explicit CycleRoute(const std::string& name, const std::vector<std::string>& stops_name,
                      const std::vector<std::shared_ptr<Stop>>& stops) : Route(name, stops_name, RouteTypes::CYCLE), stops_(stops) {
    for (const auto& elem : stops_name_) {
      unique_stops_.insert(elem);
    }
  }
  size_t CountOfStops() const override;
  size_t CountOfUniqueStops() const override;
  double RealLength() const override;
  double Length() const override;
private:
  // Порядок маршрутов с данными о остановках
  std::vector<std::shared_ptr<Stop>> stops_;
  // Хранение уникальных остановок
  std::unordered_set<std::string> unique_stops_;
};

struct RouteInfo {
  Route::RouteTypes type;
  std::string name;
  std::vector<std::string> stop_names;
};

enum class NodeType {INFO, WAIT, BUS};

struct BaseNode {
  BaseNode() = default;
  BaseNode(NodeType t) : type(t) {}
  virtual ~BaseNode() = default;
  NodeType type;
};

struct InfoNode : public BaseNode {
  using BaseNode::BaseNode;
  InfoNode() = default;
  InfoNode(NodeType t, double val) : BaseNode(t), total_time(val) {}
  std::optional<double> total_time;
};

struct WaitNode : public BaseNode {
  using BaseNode::BaseNode;
  WaitNode() = default;
  WaitNode(NodeType type, const std::string& stop_name_, double time_)
  : BaseNode(type), stop_name(stop_name_), time(time_) {}
  std::string stop_name;
  double time;
};

struct BusNode : public BaseNode {
  using BaseNode::BaseNode;
  BusNode() = default;
  BusNode(NodeType type, const std::string& route_name_, int span_count_, double time_)
  : BaseNode(type), route_name(route_name_), span_count(span_count_), time(time_) {}
  std::string route_name;
  int span_count;
  double time;
};

class Database {
  using StopData = std::unordered_map<std::string, std::shared_ptr<Stop>>;
  using RouteData = std::unordered_map<std::string, std::shared_ptr<Route>>;
  using WeightType = double;

  template <typename Weight>
  struct GraphCache {
    bool is_outdated = true;
    Graph::DirectedWeightedGraph<Weight> wgraph;
    std::unique_ptr<Graph::Router<Weight>> router = nullptr;
    Graph::VertexId curr_id = 0;
    size_t vertex_count = 0;
    // Stop Name - Route Name - Id
    std::unordered_map<std::string, std::vector<Graph::VertexId>> name_to_vertex_id;
    // Id - Stop_name, Route_name
    std::unordered_map<Graph::VertexId, std::pair<std::string, std::string>> vertex_id_to_name;
  };
  struct RoutingParam {
    double velocity;
    double waiting_time;
  };
public:
  Database();
  void AddStop(const Stop &stop);
  std::shared_ptr<Stop> TakeOrAddStop(const std::string &stop_name);
  std::shared_ptr<Stop> TakeStop(const std::string &stop_name) const;

  void AddRoute(const std::string& route_name, std::shared_ptr<Route> route);
  std::shared_ptr<Route> TakeRoute(const std::string &route_name) const;

  std::list<std::unique_ptr<BaseNode>> CreateRoute(std::string_view first_stop, std::string_view second_stop) const {
    if (gcache.is_outdated) {
      {
        //LOG_DURATION("Update graph\n");
        gcache.wgraph = UpdateGraph();
      }
      {
        //LOG_DURATION("Create router\n");
        gcache.router = std::make_unique<Graph::Router<WeightType>>(gcache.wgraph);
      }
      gcache.is_outdated = false;
    }
    //LOG_DURATION("Create route\n");
    std::list<std::unique_ptr<BaseNode>> nodes;
    auto route_info = gcache.router->BuildRoute(gcache.name_to_vertex_id.at(std::string(first_stop)).front(),
                                                gcache.name_to_vertex_id.at(std::string(second_stop)).front());
    if (!route_info) {
      nodes.push_back(std::move(std::make_unique<InfoNode>(NodeType::INFO)));
      return nodes;
    }
    nodes.push_back(std::make_unique<InfoNode>(NodeType::INFO, route_info->weight));
    int count = 0;
    double bus_time = 0;
    std::pair<std::string, std::string> from_stop, to_stop;
    for (size_t i = 0; i < route_info->edge_count; ++i) {
      auto edge_id = gcache.router->GetRouteEdge(route_info->id, i);
      auto edge = gcache.wgraph.GetEdge(edge_id);
      from_stop = gcache.vertex_id_to_name.at(edge.from);
      to_stop = gcache.vertex_id_to_name.at(edge.to);
      if (from_stop.first == to_stop.first) {
        if (count) {
          nodes.push_back(std::make_unique<BusNode>(NodeType::BUS, from_stop.second, count, bus_time));
          count = 0;
          bus_time = 0;
        }
        if (edge.weight) {
          nodes.push_back(std::make_unique<WaitNode>(NodeType::WAIT, from_stop.first, edge.weight));
        }
      }
      else {
        bus_time += edge.weight;
        count++;
      }
    }
    if (count) {
      nodes.push_back(std::make_unique<BusNode>(NodeType::BUS, from_stop.second, count, bus_time));
      count = 0;
      bus_time = 0;
    }

    //route_cache[route_info->id] = nodes;
    return nodes;
  }

  RoutingParam params_;
private:
  Graph::DirectedWeightedGraph<WeightType> UpdateGraph() const {
    auto new_wgraph = Graph::DirectedWeightedGraph<WeightType>(gcache.vertex_count);
    // Создание ребер между вершинами конец ожидания - начало ожидания
    for (const auto& [route_name, route_ptr] : routes_) {
      if (route_ptr->route_type == Route::RouteTypes::CYCLE) {
        MakeWieghtFromCycleRoute(new_wgraph, route_ptr);
      }
      if (route_ptr->route_type == Route::RouteTypes::LINEAR) {
        MakeWieghtFromLinearRoute(new_wgraph, route_ptr);
      }
    }
    return new_wgraph;
  }

  void MakeWieghtFromCycleRoute(Graph::DirectedWeightedGraph<WeightType>& graph, const std::shared_ptr<Route>& ptr) const {
    auto stop_names = ptr->GetStopsName();
    Graph::VertexId first = AddVertex(stop_names[0], ptr->GetName());
    Graph::VertexId second = 0;
    AddHubConnection(graph, first, stop_names[0]);
    for (size_t i = 1; i < stop_names.size(); ++i) {
      second = AddVertex(stop_names[i], ptr->GetName());
      graph.AddEdge({first,
                     second,
                     stops_.at(stop_names[i - 1])->distance_to_stop.at(stop_names[i]) * 1.0 / params_.velocity});
      AddHubConnection(graph, second, stop_names[i]);
      first = second;
    }
  }

  void MakeWieghtFromLinearRoute(Graph::DirectedWeightedGraph<WeightType>& graph, const std::shared_ptr<Route>& ptr) const {
    auto stop_names = ptr->GetStopsName();

    Graph::VertexId first_str = AddVertex(stop_names[0], ptr->GetName());
    Graph::VertexId second_str = 0;
    Graph::VertexId first_rev = 0;
    Graph::VertexId second_rev = gcache.name_to_vertex_id.at(stop_names[0]).front();
    AddHubConnection(graph, first_str, stop_names[0]);
    for (size_t i = 1; i < stop_names.size() - 1; ++i) {
      {
        // Добавление прямого маршрута
        second_str = AddVertex(stop_names[i], ptr->GetName());
        graph.AddEdge({first_str,
                       second_str,
                       stops_.at(stop_names[i - 1])->distance_to_stop.at(stop_names[i]) * 1.0 / params_.velocity});
        AddHubConnection(graph, second_str, stop_names[i]);
        first_str = second_str;
      }
      {
        // Добавление обратного маршрута
        first_rev = AddVertex(stop_names[i], ptr->GetName());
        graph.AddEdge({first_rev,
                       second_rev,
                       stops_.at(stop_names[i])->distance_to_stop.at(stop_names[i - 1]) * 1.0 / params_.velocity});
        AddHubConnection(graph, first_rev, stop_names[i]);
        second_rev = first_rev;
      }
    }
    graph.AddEdge({first_str,
                   gcache.name_to_vertex_id.at(stop_names[stop_names.size() - 1]).front(),
                   stops_.at(stop_names[stop_names.size() - 2])->distance_to_stop.at(stop_names[stop_names.size() - 1]) * 1.0 / params_.velocity});

    first_rev = AddVertex(stop_names[stop_names.size() - 1], ptr->GetName());
    graph.AddEdge({first_rev,
                   second_rev,
                   stops_.at(stop_names[stop_names.size() - 1])->distance_to_stop.at(stop_names[stop_names.size() - 2]) * 1.0 / params_.velocity});
    AddHubConnection(graph, first_rev, stop_names[stop_names.size() - 1]);
  }

  Graph::VertexId AddVertex(const std::string& stop_name, const std::string& route_name) const {
    gcache.name_to_vertex_id[stop_name].push_back(gcache.curr_id);
    gcache.vertex_id_to_name[gcache.curr_id++] = {stop_name, route_name};
    return gcache.name_to_vertex_id[stop_name].back();
  }

  void AddHubConnection(Graph::DirectedWeightedGraph<WeightType>& graph, Graph::VertexId vertex_to_connect, const std::string& hub_name) const {
    graph.AddEdge({gcache.name_to_vertex_id.at(hub_name).front(),
                   vertex_to_connect,
                   params_.waiting_time});
    graph.AddEdge({vertex_to_connect,
                   gcache.name_to_vertex_id.at(hub_name).front(),
                   0.0});
  }

  void IndexStops() const {
    gcache.curr_id = 0;
    gcache.vertex_count = 0;
    gcache.name_to_vertex_id.clear();
    gcache.vertex_id_to_name.clear();
    gcache.vertex_count += stops_.size();
    for (const auto& [stop_name, stop] : stops_) {
      gcache.name_to_vertex_id[stop->GetName()].push_back(gcache.curr_id);
      gcache.vertex_id_to_name[gcache.curr_id++] = {stop->GetName(), "Hub"};
    }
    for (const auto& [route_name, route] : routes_) {
      gcache.vertex_count += route->CountOfStops();
    }
  }

  StopData stops_;
  RouteData routes_;

  mutable GraphCache<WeightType> gcache;
};

class RouteBuilder {
public:
  RouteBuilder() = delete;
  explicit RouteBuilder(Database& db) : db_(db) {}
  std::shared_ptr<Route> MakeRoute(RouteInfo&& info);
private:
  Database& db_;
  // Для каждого типа маршрута своя логика создания, даже если она повторяется
  // Все что их объединяет - наличие вектора имен для остановок и имя маршрута
  std::shared_ptr<Route> MakeCycle(RouteInfo&& info);
  std::shared_ptr<Route> MakeLinear(RouteInfo&& info);
};

#endif //YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_DATABASE_H
