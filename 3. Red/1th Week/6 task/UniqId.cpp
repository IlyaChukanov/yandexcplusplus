//
// Created by ilya on 27.09.2019.
//

#include <string>
#include <vector>
using namespace std;

#define UNIQ_ID_RIGHT(num) _var_##num
#define UNIQ_ID_LEFT(num) UNIQ_ID_RIGHT(num)
#define UNIQ_ID UNIQ_ID_LEFT(__LINE__)

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}