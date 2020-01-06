//
// Created by ilya on 05.01.2020.
//

#ifndef YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_ROUTE_H
#define YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_ROUTE_H

#include <memory>
#include <list>
#include <optional>

#include "graph.h"
#include "dijkstra.h"
#include "database.h"

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
  InfoNode(double val) : BaseNode(NodeType::INFO), total_time(val) {}
  std::optional<double> total_time;
};

struct WaitNode : public BaseNode {
  using BaseNode::BaseNode;
  WaitNode() = default;
  WaitNode(const std::string& stop_name_, double time_)
      : BaseNode(NodeType::WAIT), stop_name(stop_name_), time(time_) {}
  std::string stop_name;
  double time;
};

struct BusNode : public BaseNode {
  using BaseNode::BaseNode;
  BusNode() = default;
  BusNode(const std::string& route_name_, int span_count_, double time_)
      : BaseNode(NodeType::BUS), route_name(route_name_), span_count(span_count_), time(time_) {}
  std::string route_name;
  int span_count;
  double time;
};

struct RoutingParam {
  double velocity;
  double waiting_time;
};

class DatabaseRouter {
  using WeightType = double;
  struct Edge {
    bool is_wait_edge;
    WeightType weight;
    int span_count;
    std::string bus_name;
    Graph::VertexId from;
    Graph::VertexId to;
  };
public:
  DatabaseRouter();
  DatabaseRouter(std::shared_ptr<Database> db, const RoutingParam& rp);
  void ChangeDatabase(std::shared_ptr<Database> db);
  void ChangeRoutingParams(const RoutingParam& rp);
  std::list<std::unique_ptr<BaseNode>> CreateRoute(const std::string &first_stop, const std::string &second_stop) const;
  void UpdateGraph();
  RoutingParam routing_param;
private:
  std::shared_ptr<Database> db_ = nullptr;
  bool is_outdated = true;

  void AddStops();
  void Rebase();
  void RebaseGraph();
  void RebaseRouter();

  void MakeWieghtFromCycleRoute(Graph::DirectedWeightedGraph<WeightType>& graph, const std::shared_ptr<Route>& ptr);
  void MakeWieghtFromLinearRoute(Graph::DirectedWeightedGraph<WeightType>& graph, const std::shared_ptr<Route>& ptr);

  double Velocity() const;
  std::unique_ptr<Graph::DirectedWeightedGraph<WeightType>> graph = nullptr;
  std::unique_ptr<Graph::Router<WeightType>> router = nullptr;

  Graph::VertexId curr_id = 0;
  std::unordered_map<std::string, std::pair<Graph::VertexId, Graph::VertexId>> name_to_vertex_id;
  std::unordered_map<Graph::VertexId, std::pair<std::string, std::string>> vertex_id_to_name;
  std::unordered_map<Graph::EdgeId, Edge> edges;
};

#endif //YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_ROUTE_H
