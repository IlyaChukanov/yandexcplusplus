//
// Created by ilya on 07.01.2020.
//

#ifndef YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_SVG_H
#define YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_SVG_H

#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <memory>

namespace Svg {

#define JSON_SVG
struct Point {
  double x = 0;
  double y = 0;
};

struct Rgb {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct Rgba {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  double alpha;
};

using Color = std::variant<std::monostate, std::string, Rgb, Rgba>;
const Color NoneColor{};

void RenderColor(std::ostream& out, std::monostate);
void RenderColor(std::ostream& out, const std::string& value);
void RenderColor(std::ostream& out, Rgb rgb);
void RenderColor(std::ostream& out, Rgba rgb);
void RenderColor(std::ostream& out, const Color& color);

class Object;
using SvgObjectHolder = std::unique_ptr<Object>;
template <typename ObjectType>
SvgObjectHolder MakeSvgObject(ObjectType object) {
  return std::make_unique<ObjectType>(object);
}

class Object {
public:
  virtual void Render(std::ostream& out) const = 0;
  virtual ~Object() = default;
};

template <typename Owner>
class PathProps {
public:
  Owner& SetFillColor(const Color& color) {
    fill_color_ = color;
    return AsOwner();
  }

  Owner& SetStrokeColor(const Color& color) {
    stroke_color_ = color;
    return AsOwner();
  }

  Owner& SetStrokeWidth(double value) {
    stroke_width_ = value;
    return AsOwner();
  }

  Owner& SetStrokeLineCap(const std::string& value) {
    stroke_line_cap_ = value;
    return AsOwner();
  }

  Owner& SetStrokeLineJoin(const std::string& value) {
    stroke_line_join_ = value;
    return AsOwner();
  }

  void RenderAttrs(std::ostream& out) const {
#ifdef JSON_SVG
    out << R"(fill=\")";
    RenderColor(out, fill_color_);
    out << R"(\" )";
    out << R"(stroke=\")";
    RenderColor(out, stroke_color_);
    out << R"(\" )";
    out << R"(stroke-width=\")" << stroke_width_ << R"(\" )";
    if (stroke_line_cap_) {
      out << R"(stroke-linecap=\")" << *stroke_line_cap_ << R"(\" )";
    }
    if (stroke_line_join_) {
      out << R"(stroke-linejoin=\")" << *stroke_line_join_ << R"(\" )";
    }
#else
    out << "fill=\"";
    RenderColor(out, fill_color_);
    out << "\" ";
    out << "stroke=\"";
    RenderColor(out, stroke_color_);
    out << "\" ";
    out << "stroke-width=\"" << stroke_width_ << "\" ";
    if (stroke_line_cap_) {
      out << "stroke-linecap=\"" << *stroke_line_cap_ << "\" ";
    }
    if (stroke_line_join_) {
      out << "stroke-linejoin=\"" << *stroke_line_join_ << "\" ";
    }
#endif
  }

private:
  Color fill_color_;
  Color stroke_color_;
  double stroke_width_ = 1.0;
  std::optional<std::string> stroke_line_cap_;
  std::optional<std::string> stroke_line_join_;

  Owner& AsOwner() {
    return static_cast<Owner&>(*this);
  }

};

template <typename Owner>
class TextProps {
public:
  Owner& SetFontWeight(const std::string& param) {
    font_weight = param;
    return AsOwner();
  }
  void RenderAttrs(std::ostream& out) const {
#ifdef JSON_SVG
    out << R"(font-weight=\")";
    RenderColor(out, font_weight);
    out << R"(\" )";
#else
    out << "font-weight=\"";
    RenderColor(out, font_weight);
    out << "\" ";
#endif

  }
private:
  std::string font_weight;
  Owner& AsOwner() {
    return static_cast<Owner&>(*this);
  }
};

class Circle : public Object, public PathProps<Circle> {
public:
  Circle& SetCenter(Point point);
  Circle& SetRadius(double radius);
  void Render(std::ostream& out) const override;
private:
  Point center_;
  double radius_ = 1;
};

class Polyline : public Object, public PathProps<Polyline> {
public:
  Polyline& AddPoint(Point point);
  void Render(std::ostream& out) const override;
private:
  std::vector<Point> points_;
};

class Text : public Object, public PathProps<Text> {
public:
  Text& SetPoint(Point point);
  Text& SetOffset(Point point);
  Text& SetFontSize(uint32_t size);
  Text&SetFontFamily(const std::string& value);
  Text& SetData(const std::string& data);
  void Render(std::ostream& out) const override;

protected:
  Point point_;
  Point offset_;
  uint32_t font_size_ = 1;
  std::optional<std::string> font_family_;
  std::string data_;
};

class ExtendedText : public Text, public TextProps<ExtendedText> {
public:
  ExtendedText SetFontWidth(const std::string& str);
  void Render(std::ostream& out) const override;
private:
  std::string font_width;
};

class Document : public Object {
public:
  template <typename ObjectType>
  void Add(ObjectType object) {
    objects_.push_back(std::make_unique<ObjectType>(object));
  }
  template <typename ObjectPtr>
  void AddDocPart(ObjectPtr object) {
    objects_.push_back(std::move(object));
  }
  void Render(std::ostream& out) const override;
  void Clear();
private:
  std::vector<SvgObjectHolder> objects_;
};
}

#endif //YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_SVG_H
