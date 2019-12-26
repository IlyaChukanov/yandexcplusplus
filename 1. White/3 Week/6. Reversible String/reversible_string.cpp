//
// Created by ilya on 21.08.19.
//

#include <iostream>
#include <string>

using namespace std;

class ReversibleString {
public:

  ReversibleString() = default;
  ReversibleString(const std::string& str) : string_(str) {}

  void Reverse() {
    for (int i = 0; i < string_.size() / 2; ++i) {
      char bufer = string_[i];
      string_[i] = string_[string_.size() - i - 1];
      string_[string_.size() - i - 1] = bufer;
    }
  }

  std::string ToString() const {
    return string_;
  }

private:

  std::string string_;
};

int main() {
  ReversibleString s("liveeg");
  s.Reverse();
  cout << s.ToString() << endl;

  s.Reverse();
  const ReversibleString& s_ref = s;
  string tmp = s_ref.ToString();
  cout << tmp << endl;

  ReversibleString empty;
  cout << '"' << empty.ToString() << '"' << endl;

  return 0;
}
