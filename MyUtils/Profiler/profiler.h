//
// Created by ilya on 28.09.2019.
//

#ifndef YANDEXCPLUSPLUS_MYUTILS_PROFILER_PROFILER_H_
#define YANDEXCPLUSPLUS_MYUTILS_PROFILER_PROFILER_H_

#include <chrono>
#include <iostream>
#include <string>

using namespace std;
using namespace std::chrono;

// TODO Добавить возможность остановки времени
class LogDuration {
 public:
  explicit LogDuration(const string& msg = "")
      : message(msg + ": ")
      , start(steady_clock::now())
  {}

  ~LogDuration() {
    auto finish = steady_clock::now();
    auto dur = finish - start;
    cerr << message
         << duration_cast<milliseconds>(dur).count()
         << " ms" << endl;
  }
 private:
  string message;
  steady_clock::time_point start;
};

#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message) \
  LogDuration UNIQ_ID(__LINE__){message};

#endif //YANDEXCPLUSPLUS_MYUTILS_PROFILER_PROFILER_H_