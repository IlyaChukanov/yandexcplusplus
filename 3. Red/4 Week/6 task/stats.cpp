//
// Created by ilya on 06.10.2019.
//

#include "stats.h"

Stats::Stats() {
  http_methods_ = {"GET", "POST", "PUT", "DELETE", "UNKNOWN"};
  http_uri_ = {"/", "/order", "/product", "/basket", "/help", "unknown"};
  iter_to_methods_records_.resize(METHODS_COUNT);
  iter_to_uri_records_.resize(URI_COUNT);

  // Создаем заранее map ответов и способ быстрого доступа к нужным элементам
  for (size_t i = 0; i < http_methods_.size(); ++i) {
    auto result = methods_records_.insert(std::make_pair(std::string_view(http_methods_[i]), 0));
    iter_to_methods_records_[i] = result.first;
  }
  for (size_t i = 0; i < http_uri_.size(); ++i) {
    auto result = uri_records_.insert(std::make_pair(std::string_view(http_uri_[i]), 0));
    iter_to_uri_records_[i] = result.first;
  }
}

void Stats::AddMethod(std::string_view method) {
  /*
  auto find = methods_records_.find(method);
  if (find != methods_records_.end()) {
    find->second += 1;
  }
  */
  size_t i = 0;
  for (i = 0; i < http_methods_.size(); ++i) {
    if (method == http_methods_[i]) {
      iter_to_methods_records_[i]->second += 1;
      break;
    }
  }
  if (i == http_methods_.size()) {
    iter_to_methods_records_[http_methods_.size() - 1]->second += 1;
  }
}

void Stats::AddUri(std::string_view uri) {
  /*
  auto find = uri_records_.find(uri);
  if (find != uri_records_.end()) {
    find->second += 1;
  }
  */
  std::string temp = std::string(uri);
  size_t i = 0;
  for (i = 0; i < http_uri_.size(); ++i) {
    if (uri == http_uri_[i]) {
      iter_to_uri_records_[i]->second += 1;
      break;
    }
  }
  if (i == http_uri_.size()) {
    iter_to_uri_records_[http_uri_.size() - 1]->second += 1;
  }
}

const std::map<std::string_view, int>& Stats::GetMethodStats() const {
  return methods_records_;
}

const std::map<std::string_view, int>& Stats::GetUriStats() const {
  return uri_records_;
}

HttpRequest ParseRequest(std::string_view line) {
  std::array<std::string_view, 3> data;
  size_t i = 0;
  for (;;) {
    if (line[i] == ' ') {
      ++i;
    }
    else {
      break;
    }
  }
  line.remove_prefix(i);
  for(size_t i = 0;;) {
    size_t position = line.find(' ');
    data[i] = line.substr(0, position);
    if (position == line.npos) {
      break;
    }
    else {
      i++;
      line.remove_prefix(position + 1);
    }
  }
  return {data[0], data[1], data[2]};
}