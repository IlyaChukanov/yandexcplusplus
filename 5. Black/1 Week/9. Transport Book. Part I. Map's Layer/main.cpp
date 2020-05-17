//
// Created by ilya on 25.11.2019.
//
#include "manager.h"
#include "json.h"

int main() {
  TransportDatabase::DatabaseManager dm;
  std::cout.precision(6);
  auto result = dm.ProcessAllJSONRequests();
  Json::PrintNode(result, std::cout);
  return 0;
}


