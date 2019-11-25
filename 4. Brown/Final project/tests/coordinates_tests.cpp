//
// Created by ilya on 25.11.2019.
//
#include <gtest/gtest.h>
#include <coordinates.h>

TEST(CalculateDistance, Test1) {
  {
    const Coordinates first(55.611087, 37.20829);
    const Coordinates second(55.595884, 37.209755);
    const Coordinates third(55.632761, 37.333324);
    const double result_1 = Coordinates::Distance(first, second);
    const double result_2 = Coordinates::Distance(first, second);
    const double result = result_1 + result_2;
    const double true_res = 20939.5;
    ASSERT_DOUBLE_EQ(result, true_res);
  }
}
