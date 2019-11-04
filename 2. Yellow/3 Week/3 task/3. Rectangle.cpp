//
// Created by ilya on 08.09.2019.
//

#include "3. Rectangle.h"

Rectangle::Rectangle(int width, int height) : width_(width), height_(height) {}

int Rectangle::GetArea() const {
  return width_ * height_;
}

int Rectangle::GetPerimeter() const {
  return 2 * (width_ + height_);
}

int Rectangle::GetWidth() const { return width_; }

int Rectangle::GetHeight() const { return height_; }