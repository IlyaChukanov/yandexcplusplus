//
// Created by ilya on 21.08.19.
//
#include <string>

using namespace std;

struct Specialization {
  explicit Specialization(std::string new_name) {
    name = new_name;
  }
  std::string name;
};

struct Course{
  explicit Course(std::string new_name) {
    name = new_name;
  }
  std::string name;
};

struct Week{
  explicit Week(std::string new_name) {
    name = new_name;
  }
  std::string name;
};

struct LectureTitle {
  LectureTitle() = default;
  LectureTitle(const Specialization& spec_name,
               const Course& course_name,
               const Week& week_name) {
    specialization = spec_name.name;
    course = course_name.name;
    week = week_name.name;
  }
  string specialization;
  string course;
  string week;
};

