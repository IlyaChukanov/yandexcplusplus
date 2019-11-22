//
// Created by ilya on 22.11.2019.
//
#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include "data.h"

void TestCreateDate() {
  {
    std::string date_str("2000-01-01");
    Date date("2000-01-01");
    ASSERT_EQUAL(date.GetDate(), date_str);
  }
  {
    std::string date_str("2000-12-31");
    Date date("2000-12-31");
    ASSERT_EQUAL(date.GetDate(), date_str);
  }
  {
    std::string date_str("2010-06-13");
    Date date("2010-06-13");
    ASSERT_EQUAL(date.GetDate(), date_str);
  }
}

void TestDiffDate() {
  {
    Date date_from("2000-01-01");
    Date date_to("2001-01-01");
    ASSERT_EQUAL(Date::ComputeDaysDiff(date_to, date_from), 366);
  }
  {
    Date date_from("2010-06-12");
    Date date_to("2010-07-12");
    ASSERT_EQUAL(Date::ComputeDaysDiff(date_to, date_from), 30);
  }
  {
    Date date_from("2005-12-12");
    Date date_to("2035-12-12");
    ASSERT_EQUAL(Date::ComputeDaysDiff(date_to, date_from), 10957);
  }
  {
    Date date_from("2000-01-02");
    Date date_to("2000-01-06");
    ASSERT_EQUAL(Date::ComputeDaysDiff(date_to, date_from), 4);
  }
  {
    Date date_from("2000-01-01");
    Date date_to("2099-01-01");
    ASSERT_EQUAL(Date::ComputeDaysDiff(date_to, date_from), 36160);
  }
}