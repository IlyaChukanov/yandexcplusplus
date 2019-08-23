//
// Created by ilya on 23.08.19.
//

#include <iostream>
#include <exception>
#include <string>
using namespace std;

string AskTimeServer() {
  throw runtime_error("404");
}

class TimeServer {
 public:
  string GetCurrentTime() {
    std::string new_time;
    try {
      new_time = AskTimeServer();
    }
    catch (system_error& exc) {
      return LastFetchedTime;
    }
    catch (exception& ex) {
      throw;
    }
    LastFetchedTime = new_time;
    return new_time;
  }

 private:
  string LastFetchedTime = "00:00:00";
};

int main() {
  TimeServer ts;
  try {
    cout << ts.GetCurrentTime() << endl;
  } catch (exception& e) {
    cout << "Exception got: " << e.what() << endl;
  }
  return 0;
}