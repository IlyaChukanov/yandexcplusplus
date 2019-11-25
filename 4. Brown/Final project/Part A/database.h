//
// Created by ilya on 25.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_DATABASE_H
#define YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_DATABASE_H
#include <string>

#include "coordinates.h"

class Stop {
public:

  Stop() = default;
  Stop(const std::string& name, const Coordinates& coord) : name_(name), coord_(coord) {}

  std::string GetName() const {
    return name_;
  }

  std::string& GetName() {
    return name_;
  }

  Coordinates GetCoord() const {
    return coord_;
  }

private:
  std::string name_;
  Coordinates coord_;
};

class Root {
public:

private:
  st
};

class Database {
public:

private:

};

#endif //YANDEXCPLUSPLUS_4_BROWN_FINAL_PROJECT_PART_A_DATABASE_H
