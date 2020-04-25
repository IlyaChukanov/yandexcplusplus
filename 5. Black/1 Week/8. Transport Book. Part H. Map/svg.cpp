//
// Created by ilya on 07.01.2020.
//

#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <memory>

#include "svg.h"

namespace Svg {

void RenderColor(std::ostream& out, std::monostate) {
  out << "none";
}

void RenderColor(std::ostream& out, const std::string& value) {
  out << value;
}

void RenderColor(std::ostream& out, Rgb rgb) {
  out << "rgb(" << static_cast<int>(rgb.red)
      << "," << static_cast<int>(rgb.green)
      << "," << static_cast<int>(rgb.blue) << ")";
}

void RenderColor(std::ostream& out, Rgba rgb) {
  out << "rgba(" << static_cast<int>(rgb.red)
      << "," << static_cast<int>(rgb.green)
      << "," << static_cast<int>(rgb.blue)
      << "," << rgb.alpha << ")";
}

void RenderColor(std::ostream& out, const Color& color) {
  visit([&out](const auto& value) { RenderColor(out, value); },
        color);
}


Circle& Circle::SetCenter(Point point) {
  center_ = point;
  return *this;
}
Circle& Circle::SetRadius(double radius) {
  radius_ = radius;
  return *this;
}

void Circle::Render(std::ostream& out) const {
  out << "<circle ";
  out << R"(cx=\")" << center_.x << R"(\" )";
  out << R"(cy=\")" << center_.y << R"(\" )";
  out << R"(r=\")" << radius_ << R"(\" )";
  PathProps::RenderAttrs(out);
  out << "/>";
}

Polyline& Polyline::AddPoint(Point point) {
  points_.push_back(point);
  return *this;
}

void Polyline::Render(std::ostream& out) const {
  out << "<polyline ";
  out << R"(points=\")";
  bool first = true;
  for (const Point point : points_) {
    if (first) {
      first = false;
    } else {
      out << " ";
    }
    out << point.x << "," << point.y;
  }
  out << R"(\" )";
  PathProps::RenderAttrs(out);
  out << "/>";
}

Text& Text::SetPoint(Point point) {
  point_ = point;
  return *this;
}

Text& Text::SetOffset(Point point) {
  offset_ = point;
  return *this;
}

Text& Text::SetFontSize(uint32_t size) {
  font_size_ = size;
  return *this;
}

Text& Text::SetFontFamily(const std::string& value) {
  font_family_ = value;
  return *this;
}

Text& Text::SetData(const std::string& data) {
  data_ = data;
  return *this;
}

void Text::Render(std::ostream& out) const {
  out << "<text ";
  out << R"(x=\")" << point_.x << R"(\" )";
  out << R"(y=\")" << point_.y << R"(\" )";
  out << R"(dx=\")" << offset_.x << R"(\" )";
  out << R"(dy=\")" << offset_.y << R"(\" )";
  out << R"(font-size=\")" << font_size_ << R"(\" )";
  if (font_family_) {
    out << R"(font-family=\")" << *font_family_ << R"(\" )";
  }
  PathProps::RenderAttrs(out);
  out << ">";
  out << data_;
  out << "</text>";
}

ExtendedText ExtendedText::SetFontWidth(const std::string &str) {
  font_width = str;
  return *this;
}

void ExtendedText::Render(std::ostream &out) const {
  out << "<text ";
  out << R"(x=\")" << point_.x << R"(\" )";
  out << R"(y=\")" << point_.y << R"(\" )";
  out << R"(dx=\")" << offset_.x << R"(\" )";
  out << R"(dy=\")" << offset_.y << R"(\" )";
  out << R"(font-size=\")" << font_size_ << R"(\" )";
  if (font_family_) {
    out << R"(font-family=\")" << *font_family_ << R"(\" )";
  }
  TextProps::RenderAttrs(out);
  PathProps::RenderAttrs(out);
  out << ">";
  out << data_;
  out << "</text>";
}


void Document::Render(std::ostream& out) const {
  out << R"(<?xml version=\"1.0\" encoding=\"UTF-8\" ?>)";
  out << R"(<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">)";
  for (const auto& object_ptr : objects_) {
    object_ptr->Render(out);
  }
  out << "</svg>";
}

void Document::Clear() {
  objects_.clear();
}

}
