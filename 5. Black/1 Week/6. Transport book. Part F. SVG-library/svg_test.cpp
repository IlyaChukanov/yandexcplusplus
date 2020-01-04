//
// Created by ilya on 04.01.2020.
//

#include <fstream>
#include "gtest/gtest.h"
#include "svg.cpp"

TEST(Point, DefaultConstructorTest) {
  Svg::Point a;
  ASSERT_DOUBLE_EQ(a.x, 0.0);
  ASSERT_DOUBLE_EQ(a.y, 0.0);
}

TEST(Point, InitializeTest1) {
  Svg::Point a{1.0, 2.0};
  ASSERT_DOUBLE_EQ(a.x, 1.0);
  ASSERT_DOUBLE_EQ(a.y, 2.0);
}

TEST(Point, InitializeTest2) {
  Svg::Point a = {1.0, 2.0};
  ASSERT_DOUBLE_EQ(a.x, 1.0);
  ASSERT_DOUBLE_EQ(a.y, 2.0);
}

TEST(Point, InitializeTest3) {
  auto a = Svg::Point{1.0, 2.0};
  ASSERT_DOUBLE_EQ(a.x, 1.0);
  ASSERT_DOUBLE_EQ(a.y, 2.0);
}

TEST(Point, InitializeTest4) {
  Svg::Point a;
  a.x = 1.0;
  a.y = 2.0;
  ASSERT_DOUBLE_EQ(a.x, 1.0);
  ASSERT_DOUBLE_EQ(a.y, 2.0);
}

TEST(RGB, DefaultConstructorTest) {
  Svg::Rgb a;
  ASSERT_EQ(a.red, 255);
  ASSERT_EQ(a.green, 255);
  ASSERT_EQ(a.blue, 255);
}

TEST(RGB, InitializeTest1) {
  Svg::Rgb a;
  a.red = 100;
  a.green = 0;
  a.blue = 50;
  ASSERT_EQ(a.red, 100);
  ASSERT_EQ(a.green, 0);
  ASSERT_EQ(a.blue, 50);
}

TEST(RGB, InitializeTest2) {
  Svg::Rgb a{100, 0, 50};
  ASSERT_EQ(a.red, 100);
  ASSERT_EQ(a.green, 0);
  ASSERT_EQ(a.blue, 50);
}

TEST(RGB, InitializeTest3) {
  Svg::Rgb a = {100, 0, 50};
  ASSERT_EQ(a.red, 100);
  ASSERT_EQ(a.green, 0);
  ASSERT_EQ(a.blue, 50);
}

TEST(RGB, InitializeTest4) {
  auto a = Svg::Rgb{100, 0, 50};
  ASSERT_EQ(a.red, 100);
  ASSERT_EQ(a.green, 0);
  ASSERT_EQ(a.blue, 50);
}

TEST(RGB, InitializeTest5) {
  Svg::Rgb a{255, 255, 255};
  ASSERT_EQ(a.red, 255);
  ASSERT_EQ(a.green, 255);
  ASSERT_EQ(a.blue, 255);
}

TEST(RGB, GetColorTest1) {
  Svg::Rgb a{100, 0, 50};
  auto str = a.GetColorAsString();
  ASSERT_EQ(str, "rgb(100,0,50)");
}

TEST(RGB, GetColorTest2) {
  Svg::Rgb a{100, 0, 50};
  auto str = a.GetColorAsString();
  ASSERT_EQ(str, "rgb(100,0,50)");
}

TEST(Color, DefaultConstructorTest) {
  Svg::Color a;
  ASSERT_EQ(a.GetColor(), "none");
}

TEST(Color, StringConstructorTest1) {
  Svg::Color a("red");
  ASSERT_EQ(a.GetColor(), "red");
}

TEST(Color, StringConstructorTest2) {
  Svg::Color a{"blue"};
  ASSERT_EQ(a.GetColor(), "blue");
}

TEST(Color, StringConstructorTest3) {
  Svg::Color a = {"green"};
  ASSERT_EQ(a.GetColor(), "green");
}

TEST(Color, StringConstructorTest4) {
  Svg::Color a(Svg::Rgb{100, 110, 120});
  ASSERT_EQ(a.GetColor(), "rgb(100,110,120)");
}

TEST(Color, StringConstructorTest5) {
  Svg::Color a{Svg::Rgb{100, 110, 120}};
  ASSERT_EQ(a.GetColor(), "rgb(100,110,120)");
}
TEST(Color, StringConstructorTest6) {
  auto a = Svg::Color{Svg::Rgb{255, 255, 255}};
  ASSERT_EQ(a.GetColor(), "rgb(255,255,255)");
}

std::string ColorType(const Svg::Color& c) {
  return c.GetColor();
}

TEST(Color, StringConstructorTest7) {
  ASSERT_EQ(ColorType(std::string("green")), "green");
}

TEST(Color, StringConstructorTest8) {
  ASSERT_EQ(ColorType(Svg::Rgb{100, 110, 120}), "rgb(100,110,120)");
}

TEST(Color, StringConstructorTest9) {
  ASSERT_EQ(ColorType(Svg::Rgb{255, 255, 255}), "rgb(255,255,255)");
}

TEST(Color, StringConstructorTest10) {
  ASSERT_EQ(ColorType(Svg::Rgb{0, 0, 0}), "rgb(0,0,0)");
}

TEST(Main, Test1) {

}
/*

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
*/
int main() {
  Svg::Document svg;

  svg.Add(
      Svg::Polyline{}
          .SetStrokeColor(Svg::Rgb{140, 198, 63})  // soft green
          .SetStrokeWidth(16)
          .SetStrokeLineCap("round")
          .AddPoint({50, 50})
          .AddPoint({250, 250})
  );

  for (const auto point : {Svg::Point{50, 50}, Svg::Point{250, 250}}) {
    svg.Add(
        Svg::Circle{}
            .SetFillColor("white")
            .SetRadius(6)
            .SetCenter(point)
    );
  }

  svg.Add(
      Svg::Text{}
          .SetPoint({50, 50})
          .SetOffset({10, -10})
          .SetFontSize(20)
          .SetFontFamily("Verdana")
          .SetFillColor("black")
          .SetData("C")
  );
  svg.Add(
      Svg::Text{}
          .SetPoint({250, 250})
          .SetOffset({10, -10})
          .SetFontSize(20)
          .SetFontFamily("Verdana")
          .SetFillColor("black")
          .SetData("C++")
  );
  std::ofstream a("a.svg");
  if (!a) std::cout << "Файл НЕ создан" << std::endl;
  svg.Render(a);
}