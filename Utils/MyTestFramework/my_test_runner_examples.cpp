//
// Created by ilya on 12.09.2019.
//

#include my_test_runner.h

UnitTestList SomeTest() {
  UnitTestList us("Part 1 test");
  us.AddTest(
      UnitTest(
          TestName("SomeName"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            Assert(input, output, Equal<type, type>, desc);
          })
  );
  return us;
}

void Tests() {
  UnitTestManager um;
  um.Add(SomeTest());
  um.Run();
}