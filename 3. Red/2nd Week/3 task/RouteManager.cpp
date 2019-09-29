//
// Created by ilya on 29.09.2019.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class RouteManager {
 public:
  void AddRoute(int start, int finish) {
    reachable_lists_[start].push_back(finish);  // O(log N)
    reachable_lists_[finish].push_back(start);  // O(log N)
  }

  int FindNearestFinish(int start, int finish) const {
    int result = abs(start - finish);
    if (reachable_lists_.count(start) < 1) {    // O(log N)
      return result;
    }
    const std::vector<int>& reachable_stations = reachable_lists_.at(start);
    if (!reachable_stations.empty()) {
      result = std::min(result, abs(finish - *min_element(begin(reachable_stations), end(reachable_stations),
              [finish](int lhs, int rhs) { return abs(lhs - finish) < abs(rhs - finish); }
          ))    // O(N)
      );
    }
    return result;
  }
 private:
  std::map<int, std::vector<int>> reachable_lists_;
};

int main() {
  RouteManager routes;
  int query_count;
  std::cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    std::string query_type;
    std::cin >> query_type;
    int start, finish;
    std::cin >> start >> finish;
    if (query_type == "ADD") {
      routes.AddRoute(start, finish);
    }
    else if (query_type == "GO") {
      std::cout << routes.FindNearestFinish(start, finish) << "\n";
    }
  }

  return 0;
}