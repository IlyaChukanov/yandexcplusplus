//
// Created by ilya on 04.01.2020.
//

#ifndef YANDEXCPLUSPLUS_5_BLACK_1_WEEK_6_TRANSPORT_BOOK_PART_F_SVG_LIBRARY_SVG_H
#define YANDEXCPLUSPLUS_5_BLACK_1_WEEK_6_TRANSPORT_BOOK_PART_F_SVG_LIBRARY_SVG_H
#include <string>
#include <vector>

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <iostream>
#include <optional>

namespace Svg {

struct Point {
  Point() : x(0), y(0) {}
  Point(double x_, double y_) : x(x_), y(y_) {}
  std::string GetPointAsString() const {
    return std::to_string(x) + "," + std::to_string(y);
  }
  std::string GetXAsString() const {
    return std::to_string(x);
  }
  std::string GetYAsString() const {
    return std::to_string(y);
  }
  double x;
  double y;
};

struct Rgb {
  Rgb() : red(255), green(255), blue(255) {}
  Rgb(uint8_t red_, uint8_t green_, uint8_t blue_) : red(red_), green(green_), blue(blue_) {}
  std::string GetColorAsString() const {
    return "rgb(" + std::to_string(red) + "," + std::to_string(green) + "," + std::to_string(blue) + ")";
  }
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

class Color {
  const static std::string none_color;
public:
  Color() : color_("none") {}
  Color(const std::string& color) : color_(color) {}
  Color(const char* color) : color_(color) {}
  Color(const Rgb& rgb_color) : color_(rgb_color.GetColorAsString()) {
    rgb_data = rgb_color;
  }
  std::string GetColor() const {
    return color_;
  }
private:
  std::string color_;
  Rgb rgb_data;
};
const Color NoneColor = Color();

class Figure {
public:
  Figure() = default;
  Figure(const Figure& other) = default;
  Figure(Figure&& other) noexcept = default;
  Figure& operator=(const Figure& other) = default;
  Figure& operator=(Figure&& other) noexcept = default;

  virtual ~Figure() = default;
  Figure& SetFillColor(Color color) {
    fill_color = color;
    return *this;
  }
  Figure& SetStrokeColor(Color color) {
    stroke_color = color;
    return *this;
  }
  Figure& SetStrokeWidth(double w) {
    width = w;
    return *this;
  }
  Figure& SetStrokeLineCap(std::string cap) {
    stroke_line_cap = cap;
    return *this;
  }
  Figure& SetStrokeLineJoin(std::string join) {
    stroke_line_cap = join;
    return *this;
  }


  friend std::ostream& operator<<(std::ostream& stream, const Figure& figure) {
    figure.WriteInStream(stream);
    return stream;
  }

protected:
  virtual std::ostream& WriteInStream(std::ostream& stream) const {
    stream << "fill = \"" << fill_color.GetColor() << "\" ";
    stream << "stroke = \"" << stroke_color.GetColor() << "\" ";
    stream << "stroke-width = \"" << width << "\" ";
    if (stroke_line_cap) {
      stream << "stroke-linecap = \"" << stroke_line_cap.value() << "\" ";
    }
    if (stroke_line_join) {
      stream << "stroke-linejoin = \"" << stroke_line_join.value() << "\" ";
    }
    return stream;
  }

private:
  Color fill_color = NoneColor;
  Color stroke_color = NoneColor;
  double width = 1.0;
  std::optional<std::string> stroke_line_cap;
  std::optional<std::string> stroke_line_join;

};

template <typename Derived>
class FigureAdapter : public Figure {
public:
  Derived& SetFillColor(Color color) & {
    return static_cast<Derived&>(Figure::SetFillColor(std::move(color)));
  };
  Derived& SetStrokeColor(Color color) & {
    return static_cast<Derived&>(Figure::SetStrokeColor(std::move(color)));
  };
  Derived& SetStrokeWidth(double width) & {
    return static_cast<Derived&>(Figure::SetStrokeWidth(width));
  };
  Derived& SetStrokeLineCap(std::string source) & {
    return static_cast<Derived&>(Figure::SetStrokeLineCap(std::move(source)));
  };
  Derived& SetStrokeLineJoin(std::string source) & {
    return static_cast<Derived&>(Figure::SetStrokeLineJoin(std::move(source)));
  };

  Derived&& SetFillColor(Color color) && {
    return static_cast<Derived&&>(Figure::SetFillColor(std::move(color)));
  };
  Derived&& SetStrokeColor(Color color) && {
    return static_cast<Derived&&>(Figure::SetStrokeColor(std::move(color)));
  };
  Derived&& SetStrokeWidth(double width) && {
    return static_cast<Derived&&>(Figure::SetStrokeWidth(width));
  };
  Derived&& SetStrokeLineCap(std::string source) && {
    return static_cast<Derived&&>(Figure::SetStrokeLineCap(std::move(source)));
  };
  Derived&& SetStrokeLineJoin(std::string source) && {
    return static_cast<Derived&&>(Figure::SetStrokeLineJoin(std::move(source)));
  };
};

class Circle : public FigureAdapter<Circle> {
public:
  Circle() = default;
  Circle(const Circle& other) = default;
  Circle(Circle&& other) noexcept = default;
  Circle& operator=(const Circle& other) = default;
  Circle& operator=(Circle&& other) noexcept = default;

  Circle& SetCenter(Point p) {
    center = p;
    return *this;
  }
  Circle& SetRadius(double r) {
    radius = r;
    return *this;
  }
private:
  std::ostream& WriteInStream(std::ostream& stream) const override {
    stream << "<circle ";
    Figure::WriteInStream(stream);
    stream << "cx = \"" << center.x << "\" ";
    stream << "cy = \"" << center.y << "\" ";
    stream << "r = \"" << radius << "\"";
    stream << "/>";
    return stream;
  }
  Point center;
  double radius = 1.0;
};

class Polyline : public FigureAdapter<Polyline> {
public:
  Polyline() = default;
  Polyline(const Polyline& other) = default;
  Polyline(Polyline&& other) noexcept = default;
  Polyline& operator=(const Polyline& other) = default;
  Polyline& operator=(Polyline&& other) noexcept = default;

  Polyline& AddPoint(Point p) {
    points_.push_back(p);
    return *this;
  }
private:
  std::string GetPoints() const {
    if (points_.empty()) {
      return "";
    }
    std::stringstream s;
    for (size_t i = 0; i < points_.size(); ++i) {
      if (i) {
        s << " ";
      }
      s << points_[i].GetPointAsString();
    }
    return s.str();
  }

  std::ostream& WriteInStream(std::ostream& stream) const override {
    stream << "<polyline ";
    Figure::WriteInStream(stream);
    stream << "points = \"" << GetPoints() << "\"";
    stream << "/>";
    return stream;
  }
  std::vector<Point> points_;
};

class Text : public FigureAdapter<Text> {
public:
  Text() = default;
  Text(const Text& other) = default;
  Text(Text&& other) noexcept = default;
  Text& operator=(const Text& other) = default;
  Text& operator=(Text&& other) noexcept = default;

  Text& SetPoint(Point p) {
    start_point = p;
    return *this;
  }
  Text& SetOffset(Point p) {
    offset = p;
    return *this;
  }
  Text& SetFontSize(uint32_t size) {
    font_size = size;
    return *this;
  }
  Text& SetFontFamily(const std::string& family) {
    font_family = family;
    return *this;
  }
  Text& SetFontFamily(std::string&& family) {
    font_family = std::move(family);
    return *this;
  }
  Text& SetData(const std::string& data_) {
    data = data_;
    return *this;
  }
  Text& SetData(std::string&& data_) {
    data = std::move(data_);
    return *this;
  }

private:
  std::ostream& WriteInStream(std::ostream& stream) const override {
    stream << "<text ";
    Figure::WriteInStream(stream);
    stream << "x = \"" << start_point.x << "\" ";
    stream << "y = \"" << start_point.y << "\" ";
    stream << "dx = \"" << offset.x << "\" ";
    stream << "dy = \"" << offset.y << "\" ";
    stream << "font-size = \"" << font_size << "\" ";
    if (font_family) {
      stream << "font-family = \"" << font_family.value() << "\" ";
    }
    stream << ">" << data;
    stream << "</text>";
    return stream;
  }
  Point start_point;
  Point offset;
  uint32_t font_size = 1;
  std::optional<std::string> font_family;
  std::string data;
};

class Document {
public:
  Document() = default;

  template <typename FigureObj>
  Document& Add(const FigureObj& obj) {
    std::cerr << "Start adding" << std::endl;
    figures_.push_back(std::make_unique<FigureObj>(obj));
    std::cerr << "End adding" << std::endl;
    return *this;
  }
  void Render(std::ostream& out) const {
    std::cerr << "Start render" << std::endl;
    out << R"(<?xml version="1.0" encoding="UTF-8" ?>)"
        << R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1">)";
    for (const auto& ptr : figures_) {
      out << *ptr;
    }
    out << "</svg>";
  }
private:
  std::vector<std::unique_ptr<Figure>> figures_;
};
}


#endif //YANDEXCPLUSPLUS_5_BLACK_1_WEEK_6_TRANSPORT_BOOK_PART_F_SVG_LIBRARY_SVG_H
