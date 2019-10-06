//
// Created by ilya on 06.10.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_4RD_WEEK_6_TASK_HTTP_REQUEST_H_
#define YANDEXCPLUSPLUS_3_RED_4RD_WEEK_6_TASK_HTTP_REQUEST_H_

#include <string_view>

struct HttpRequest {
  std::string_view method, uri, protocol;
};

#endif //YANDEXCPLUSPLUS_3_RED_4RD_WEEK_6_TASK_HTTP_REQUEST_H_
