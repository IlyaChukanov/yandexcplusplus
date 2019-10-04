//
// Created by ilya on 04.10.2019.
//

#include "Translator.h"
#include "../../../MyUtils/MyTestFramework/TestFramework.h"



using namespace std;

void TestSimple() {
  Translator translator;
  translator.Add(string("stol"), string("table"));
  translator.Add(string("okno"), string("window"));
  translator.Add(string("abc"), string("cbd"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}