//
// Created by ilya on 21.08.19.
//

class Incognizable {
public:
  Incognizable() {
    first_ = 0;
    second_ = 0;
  }
  Incognizable(int first) {
    first_ = first;
  }
  Incognizable(int first, int second) {
    first_ = first;
    second_ = second_;
  }
private:
  int first_, second_;
};

int main() {
  Incognizable a;
  Incognizable b = {};
  Incognizable c = {0};
  Incognizable d = {0, 1};
  return 0;
}