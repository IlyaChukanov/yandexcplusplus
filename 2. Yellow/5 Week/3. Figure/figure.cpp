//
// Created by ilya on 15.09.2019.
//

#include <memory>
#include <iostream>
#include <iomanip>

#include "Figure.h"

std::string Figure::Name() const  {
  return name_;
}

Rect::Rect(const std::string &name, double width, double height)
    : Figure(name)
    , width_(width)
    , heigth_(height) {}

double Rect::Perimeter() const {
  return 2.0 * (width_ + heigth_);
}

double Rect::Area() const {
  return width_ * heigth_;
}

Triangle::Triangle(const std::string& name, double a, double b, double c)
    : Figure(name)
    , first_side_(a)
    , second_side_(b)
    , third_side_(c) {}

double Triangle::Perimeter() const {
  return first_side_ + second_side_ + third_side_;
}

double Triangle::Area() const {
  double p = Perimeter() / 2.0;
  return std::sqrt(p * (p - first_side_) * (p - second_side_) * (p - third_side_));
}

Circle::Circle(const std::string& name, double r)
    : Figure(name)
    , radius_(r) {}

double Circle::Perimeter() const  {
  return radius_  * 2.0 * 3.14;
}

double Circle::Area() const  {
  return 3.14 * radius_ * radius_;
}

std::shared_ptr<Figure> CreateFigure(std::istringstream& is) {
  std::string type;
  is >> type;
  if (type == "RECT") {
    double width, height;
    is >> width >> height;
    return std::make_shared<Rect>(type, width, height);
  }
  if (type == "TRIANGLE") {
    double a, b, c;
    is >> a >> b >> c;
    return std::make_shared<Triangle>(type, a, b, c);
  }
  if (type == "CIRCLE") {
    double r;
    is >> r;
    return std::make_shared<Circle>(type, r);
  }

  std::string base = "None type (Rect)";
  double base_arg = 0.0;
  return std::make_shared<Rect>(base, base_arg, base_arg);
}

