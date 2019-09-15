//
// Created by ilya on 14.09.2019.
//

#include <iostream>
#include <string>

using namespace std;

class Animal {
 public:

  Animal(const std::string& name) : Name(name) {}

  const string Name;
};


class Dog : public Animal {
 public:

  Dog(const std::string& name) : Animal(name) {}

  void Bark() {
    cout << Name << " barks: woof!" << endl;
  }
};
