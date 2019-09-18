//
// Created by ilya on 07.09.2019.
//
#include "2. Phone Number.h"
#include "../../../MyUtils/MyTestFramework/MyTestFramework.h"

UnitTestList TestGoodFormat() {
  UnitTestList us("Good format");
  us.AddTest(
      UnitTest(
          TestName("Full format +C-XXX-ZZZZZZ"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "+C-XXX-ZZZZZZ";
            try {
              PhoneNumber ph("+C-XXX-ZZZZZZ");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +9-888-666666"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "+9-888-666666";
            try {
              PhoneNumber ph("+9-888-666666");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +9-cpp-course"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "+9-cpp-course";
            try {
              PhoneNumber ph("+9-cpp-course");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +9-888-222-33-33-33"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "+9-888-222-33-33-33";
            try {
              PhoneNumber ph("+9-888-222-33-33-33");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  return us;
}

UnitTestList TestBadFormat() {
  UnitTestList us("Bade format");
  us.AddTest(
      UnitTest(
          TestName("Full format 9-888-222"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "First character is not '+'";
            try {
              PhoneNumber ph("9-888-222");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format 9-888"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "First character is not '+'";
            try {
              PhoneNumber ph("9-888");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format "),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "First character is not '+'";
            try {
              PhoneNumber ph("");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +9-888"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "Not enough codes";
            try {
              PhoneNumber ph("+9-888");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +9"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "Not enough codes";
            try {
              PhoneNumber ph("+9");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "Not enough codes";
            try {
              PhoneNumber ph("+");
              result = ph.GetInternationalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  return us;
}

UnitTestList TestPartFormat() {
  UnitTestList us("Part test");
  us.AddTest(
      UnitTest(
          TestName("Full format +9-888-777777"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "9";
            try {
              PhoneNumber ph("+9-888-777777");
              result = ph.GetCountryCode();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +9-888-777777"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "888";
            try {
              PhoneNumber ph("+9-888-777777");
              result = ph.GetCityCode();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +9-888-777777"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "777777";
            try {
              PhoneNumber ph("+9-888-777777");
              result = ph.GetLocalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +X-YYY-ZZZZZZ"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "X";
            try {
              PhoneNumber ph("+X-YYY-ZZZZZZ");
              result = ph.GetCountryCode();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +X-YYY-ZZZZZZ"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "YYY";
            try {
              PhoneNumber ph("+X-YYY-ZZZZZZ");
              result = ph.GetCityCode();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  us.AddTest(
      UnitTest(
          TestName("Full format +X-YYY-ZZZZZZ"),
          TestDescription("equal"),
          HintDescription(""),
          [](const std::string& desc) {
            std::string result, answer = "ZZZZZZ";
            try {
              PhoneNumber ph("+X-YYY-ZZZZZZ");
              result = ph.GetLocalNumber();
            }
            catch (std::exception &e) {
              result = e.what();
            }
            Assert(result, answer, Equal<std::string, std::string>, desc);
          })
  );
  return us;
}

void Tests() {
  UnitTestManager um;
  um.Add(TestGoodFormat());
  um.Add(TestBadFormat());
  um.Add(TestPartFormat());
  um.Run();
}

int main() {
  Tests();
  return 0;
}