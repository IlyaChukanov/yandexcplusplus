//
// Created by ilya on 12.09.2019.
//

#include "../../../MyUtils/MyTestFramework.h"
#include "ArithmeticExpresion.h"

UnitTestList FullExpr() {
  UnitTestList us("Part 1 test");
  us.AddTest(
      UnitTest(
          TestName("Test full 1"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            ArithmeticExpresion test(5);
            test.AddOperations({"+", 1});
            test.AddOperations({"-", 2});
            test.AddOperations({"*", 3});
            test.AddOperations({"/", 4});
            Assert(test.FullForm(), "((((5) + 1) - 2) * 3) / 4", Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Test full 2"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            ArithmeticExpresion test(5);
            test.AddOperations({"+", 1});
            test.AddOperations({"-", -2});
            test.AddOperations({"*", 3});
            test.AddOperations({"/", -4});
            Assert(test.FullForm(), "((((5) + 1) - -2) * 3) / -4", Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Test full 2"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            ArithmeticExpresion test(5);
            test.AddOperations({"+", 1});
            test.AddOperations({"-", -2});
            test.AddOperations({"*", 3});
            test.AddOperations({"/", -4});
            test.AddOperations({"+", 5});
            Assert(test.FullForm(), "(((((5) + 1) - -2) * 3) / -4) + 5", Equal<std::string, std::string>, desc);
          })
  );
  return us;
}

UnitTestList NormExpr() {
  UnitTestList us("Part 1 test");
  us.AddTest(
      UnitTest(
          TestName("Test norm 1"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            ArithmeticExpresion test(5);
            test.AddOperations({"+", 1});
            test.AddOperations({"-", 2});
            test.AddOperations({"*", 3});
            test.AddOperations({"/", 4});
            Assert(test.NormalForm(), "(5 + 1 - 2) * 3 / 4", Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Test norm 2"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            ArithmeticExpresion test(5);
            test.AddOperations({"*", 1});
            test.AddOperations({"-", -2});
            test.AddOperations({"/", 3});
            Assert(test.NormalForm(), "(5 * 1 - -2) / 3", Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Test norm 2"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            ArithmeticExpresion test(4);
            test.AddOperations({"*", 4});
            test.AddOperations({"/", 4});
            test.AddOperations({"*", 3});
            test.AddOperations({"+", 1});
            Assert(test.NormalForm(), "4 * 4 / 4 * 3 + 1", Equal<std::string, std::string>, desc);
          })
  );
  return us;
}

void Tests() {
 UnitTestManager um;
 um.Add(FullExpr());
 um.Add(NormExpr());
 um.Run();
}

int main() {
  Tests();
  return 0;
}