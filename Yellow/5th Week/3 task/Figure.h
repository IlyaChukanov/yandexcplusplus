//
// Created by ilya on 15.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_5TH_WEEK_3_TASK_FIGURE_H_
#define SECONDTWEEK_YELLOW_5TH_WEEK_3_TASK_FIGURE_H_

#include <string>
#include <iostream>
#include <cmath>

class Figure {
 public:
  Figure(const std::string& name) : name_(name) {}

  virtual std::string Name() const;

  virtual double Perimeter() const = 0;
  virtual double Area() const = 0;
 private:
  std::string name_;
};

class Rect : public Figure {
 public:
  Rect(const std::string& name, double width, double height);
  double Perimeter() const override;
  double Area() const override;
 private:
  double width_, heigth_;
};

class Triangle : public Figure {
 public:
  Triangle(const std::string& name, double a, double b, double c);
  double Perimeter() const override;
  double Area() const override;
 private:
  double first_side_, second_side_, third_side_;
};

class Circle : public Figure {
 public:
  Circle(const std::string& name, double r);
  double Perimeter() const override;
  double Area() const override;
 private:
  double radius_;
};

std::shared_ptr<Figure> CreateFigure(std::istringstream& is);

#endif //SECONDTWEEK_YELLOW_5TH_WEEK_3_TASK_FIGURE_H_
