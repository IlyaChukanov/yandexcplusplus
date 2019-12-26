//
// Created by ilya on 08.09.2019.
//

#ifndef SECONDTWEEK_YELLOW_3RD_WEEK_3_RECTANGLE_H_
#define SECONDTWEEK_YELLOW_3RD_WEEK_3_RECTANGLE_H_

class Rectangle {
 public:

  Rectangle(int width, int height);

  int GetArea() const;

  int GetPerimeter() const;

  int GetWidth() const;

  int GetHeight() const;

 private:
  int width_, height_;
};

#endif //SECONDTWEEK_YELLOW_3RD_WEEK_3_RECTANGLE_H_
