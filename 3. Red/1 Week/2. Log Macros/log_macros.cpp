//
// Created by ilya on 26.09.2019.
//

//#include "../../../Utils/MyTestFramework/TestFramework.h"
#include <sstream>
#include <string>
using namespace std;

class Logger {
 public:
  explicit Logger(ostream& output_stream) : os(output_stream) {}

  void SetLogLine(bool value) { log_line = value; }
  void SetLogFile(bool value) { log_file = value; }

  void Log(const string& message) {
    os << message << std::endl;
  }

  bool IsLogLine() { return log_line; }
  bool IsLogFile() { return log_file; }

 private:
  ostream& os;
  bool log_line = false;
  bool log_file = false;
};

#define LOG(logger, message) {  \
  std::stringstream result;     \
  if (logger.IsLogFile()        \
     && logger.IsLogLine()) {   \
    result <<  __FILE__ << ":"; \
    result << __LINE__ << " ";  \
  }                             \
  else if (logger.IsLogLine()) {\
    result << "Line ";          \
    result << __LINE__ << " ";  \
  }                             \
  else if (logger.IsLogFile()) {\
    result << __FILE__ << " ";  \
  }                             \
  result << message;            \
  logger.Log(result.str());     \
}
/*
void TestLog() {
#line 1 "logger.cpp"

  ostringstream logs;
  Logger l(logs);
  LOG(l, "hello");

  l.SetLogFile(true);
  LOG(l, "hello");

  l.SetLogLine(true);
  LOG(l, "hello");

  l.SetLogFile(false);
  LOG(l, "hello");

  string expected = "hello\n";
  expected += "logger.cpp hello\n";
  expected += "logger.cpp:10 hello\n";
  expected += "Line 13 hello\n";
  ASSERT_EQUAL(logs.str(), expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLog);
}
*/