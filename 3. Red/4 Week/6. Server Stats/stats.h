//
// Created by ilya on 06.10.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_4RD_WEEK_6_TASK_STATS_H_
#define YANDEXCPLUSPLUS_3_RED_4RD_WEEK_6_TASK_STATS_H_

#include "http_request.h"

#include <string_view>
#include <map>
#include <vector>
#include <array>

class Stats {
 public:

  Stats();

  void AddMethod(std::string_view method);
  void AddUri(std::string_view uri);
  const std::map<std::string_view, int>& GetMethodStats() const;
  const std::map<std::string_view, int>& GetUriStats() const;
 private:

  // Количество включает наличие неопределенного method или uri
  static const uint64_t METHODS_COUNT = 5;
  static const uint64_t URI_COUNT = 6;

  std::array<std::string, METHODS_COUNT> http_methods_;
  std::array<std::string, URI_COUNT> http_uri_;

  std::vector<std::map<std::string_view, int>::iterator> iter_to_methods_records_;
  std::vector<std::map<std::string_view, int>::iterator> iter_to_uri_records_;

  std::map<std::string_view, int> methods_records_;
  std::map<std::string_view, int> uri_records_;
};

HttpRequest ParseRequest(std::string_view line);

#endif //YANDEXCPLUSPLUS_3_RED_4RD_WEEK_6_TASK_STATS_H_
