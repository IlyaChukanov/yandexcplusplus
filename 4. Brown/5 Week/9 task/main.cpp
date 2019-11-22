//
// Created by ilya on 22.11.2019.
//
#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include "budget_test.h"

void TestAll() {
  TestRunner tr;
  RUN_TEST(tr, TestCreateDate);
  RUN_TEST(tr, TestDiffDate);
}

int main() {
  TestAll();
}