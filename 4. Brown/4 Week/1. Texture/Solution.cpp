#include "Common.h"

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`
#include <iostream>

// Точка передаётся в локальных координатах
bool IsPointInSize(Point p, Size s) {
  return p.x >= 0 && p.y >= 0 && p.x < s.width && p.y < s.height;
}

Size GetImageSize(const Image& image) {
  auto width = static_cast<int>(image.empty() ? 0 : image[0].size());
  auto height = static_cast<int>(image.size());
  return {width, height};
}

class Shape : public IShape {
 public:
  Shape() {
    size_ = {0, 0};
    position_ = {0, 0};
  }

  std::unique_ptr<IShape> Clone() const override = 0;

  void SetPosition(Point point) override {
    position_ = point;
  }

  Point GetPosition() const  override {
    return position_;
  }

  void SetSize(Size size) override {
    size_ = size;
  }

  Size GetSize() const override {;
    return size_;
  }

  void SetTexture(std::shared_ptr<ITexture> texture) override {
    texture_ = texture;
  }

  ITexture* GetTexture() const override {
    return texture_.get();
  }

  // Рисует фигуру на указанном изображении
  void Draw(Image& img) const override {
    Point p;
    auto image_size = GetImageSize(img);
    for (p.y = 0; p.y < size_.height; ++p.y) {
      for (p.x = 0; p.x < size_.width; ++p.x) {
        if (IsPointInShape(p)) {
          char pixel = '.';
          if (texture_ && IsPointInSize(p, texture_->GetSize())) {
            pixel = texture_->GetImage()[p.y][p.x];
          }
          Point dp = {position_.x + p.x, position_.y + p.y};
          if (IsPointInSize(dp, image_size)) {
            img[dp.y][dp.x] = pixel;
          }
        }
      }
    }
  }

 protected:
  virtual bool IsPointInShape(Point) const = 0;
  Size size_;
  Point position_;
  std::shared_ptr<ITexture> texture_;

};

namespace Shapes {
class Rectangle : public Shape {
 public:
  unique_ptr<IShape> Clone() const override {
    return make_unique<Rectangle>(*this);
  }

 private:
  bool IsPointInShape(Point p) const override {
    return true;
  }
};

class Ellipse : public Shape {
 public:
  unique_ptr<IShape> Clone() const override {
    return make_unique<Ellipse>(*this);
  }

 private:
  bool IsPointInShape(Point p) const override {
    return IsPointInEllipse(p, GetSize());
  }
};
}

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
  switch (shape_type) {
    case ShapeType::Rectangle: {
      return std::make_unique<Shapes::Rectangle>();
    }
    case ShapeType::Ellipse: {
      return std::make_unique<Shapes::Ellipse>();
    }
    default: {
      throw std::logic_error("Wrong shape type");
    }
  }
}
