//
// Created by ilya on 28.11.2019.
//

#ifndef YANDEXBROWNFINAL_4_BROWN_FINAL_PROJECT_PART_A_MANAGER_H
#define YANDEXBROWNFINAL_4_BROWN_FINAL_PROJECT_PART_A_MANAGER_H
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <list>

#include "json.h"
#include "request.h"
#include "database.h"
#include "route.h"
#include "map.h"

namespace TransportDatabase {
void PrintResults(const std::vector<std::string> &results, std::ostream &out = std::cout);
class DatabaseManager {
public:
  DatabaseManager();
  explicit DatabaseManager(std::shared_ptr<Database> db);
  void ChangeDatabase(std::shared_ptr<Database> db);
  Json::Node ProcessAllJSONRequests(std::istream &in = std::cin);
  Json::Node ProcessJSONReadRequest(const Json::Node &node);
  Json::Node ProcessJSONModifyRequest(const Json::Node &node);
private:

  RenderParams ExtractRenderParams(const Json::Node& node);

  Json::Node MakeJSONAnswerFromAnyRequest(RequestHolder request);
  RequestHolder ParseModifyJSONRequest(const Json::Node &node);
  RequestHolder ParseReadJSONRequest(const Json::Node &node);

  template<typename RequestType>
  RequestHolder ParseRequest(RequestType type, std::string_view request_str);
  template<typename RequestType>
  RequestHolder JSONRequest(RequestType type, const Json::Node &node);

  std::shared_ptr<Database> db_;
  std::shared_ptr<Router> router;
  std::shared_ptr<Map> render;
};
}
#endif //YANDEXBROWNFINAL_4_BROWN_FINAL_PROJECT_PART_A_MANAGER_H
