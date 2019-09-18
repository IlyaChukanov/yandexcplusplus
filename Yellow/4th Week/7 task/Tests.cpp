//
// Created by ilya on 10.09.2019.
//

#include <algorithm>
#include "../../../MyUtils/MyTestFramework/MyTestFramework.h"
#include "MergeSortPart2.cpp"

UnitTestList Part1() {
  UnitTestList us("Part 1 test");
  us.AddTest(
      UnitTest(
          TestName("{1, 5, 2, 3}"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::vector<int> input = {1, 5, 2, 3};
            std::vector<int> output = input;

            std::sort(output.begin(), output.end());
            MergeSort(input.begin(), input.end());
            Assert(input, output, Equal<std::vector<int>, std::vector<int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("{1, 5, 2, 3, 4, 11, 13, 21}"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::vector<int> input = {1, 5, 2, 3, 4, 11, 13, 21};
            std::vector<int> output = input;

            std::sort(output.begin(), output.end());
            MergeSort(input.begin(), input.end());
            Assert(input, output, Equal<std::vector<int>, std::vector<int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("{1, 5}"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::vector<int> input = {1, 5};
            std::vector<int> output = input;

            std::sort(output.begin(), output.end());
            MergeSort(input.begin(), input.end());
            Assert(input, output, Equal<std::vector<int>, std::vector<int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("{}"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::vector<int> input = {};
            std::vector<int> output = input;

            std::sort(output.begin(), output.end());
            MergeSort(input.begin(), input.end());
            Assert(input, output, Equal<std::vector<int>, std::vector<int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("{1, 2, 3, 7, 8, 9}"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::vector<int> input = {1, 2, 3, 7, 8, 9};
            std::vector<int> output = input;

            std::sort(output.begin(), output.end());
            MergeSort(input.begin(), input.end());
            Assert(input, output, Equal<std::vector<int>, std::vector<int>>, desc);
          })
  );
  return us;
}

UnitTestList Part2() {
  UnitTestList us("Part 2 test");
  us.AddTest(
      UnitTest(
          TestName("{1, 5, 2}"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::vector<int> input = {1, 5, 2};
            std::vector<int> output = input;

            std::sort(output.begin(), output.end());
            MergeSort(input.begin(), input.end());
            Assert(input, output, Equal<std::vector<int>, std::vector<int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("{6, 4, 7, 6, 4, 4, 0, 1, 5}"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::vector<int> input = {6, 4, 7, 6, 4, 4, 0, 1, 5};
            std::vector<int> output = input;

            std::sort(output.begin(), output.end());
            MergeSort(input.begin(), input.end());
            Assert(input, output, Equal<std::vector<int>, std::vector<int>>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("{6, 4, 7, 6}"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::vector<int> input = {6, 4, 7, 6};
            std::vector<int> output = input;

            std::sort(output.begin(), output.end());
            MergeSort(input.begin(), input.end());
            Assert(input, output, Equal<std::vector<int>, std::vector<int>>, desc);
          })
  );
  return us;
}
void Tests() {
  UnitTestManager um;
  um.Add(Part1());
  um.Add(Part2());
  um.Run();
}

int main() {
  Tests();
  return 0;
}