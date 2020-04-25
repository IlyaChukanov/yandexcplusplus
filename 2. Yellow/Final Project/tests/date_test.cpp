//
// Created by ilya on 18.09.2019.
//

#include <gtest/gtest.h>
#include <sstream>
#include "date.h"


TEST(DateConstructorTest, DefaultConstructor) {
  {
    Date date;
    EXPECT_EQ(date.GetDate(), "0000-00-00");
  }
}

TEST(DateConstructorTest, StringTestGood) {
  {
    std::istringstream is("2017-02-02");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "2017-02-02");
  }
  {
    std::istringstream is("2017-1-9");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "2017-01-09");
  }
  {
    std::istringstream is("2017-9-1");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "2017-09-01");
  }
  {
    std::istringstream is("0-01-01");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "0000-01-01");
  }
  {
    std::istringstream is("9999-01-01");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "9999-01-01");
  }
  {
    std::istringstream is("11111-01-01");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "11111-01-01");
  }
  {
    std::istringstream is("2019-6-15");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "2019-06-15");
  }
  {
    std::istringstream is("2019-12-15");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "2019-12-15");
  }
  {
    std::istringstream is("2019-2-31");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "2019-02-31");
  }
  {
    std::istringstream is("2019-12-31");
    Date date(is.str());
    EXPECT_EQ(date.GetDate(), "2019-12-31");
  }
}

TEST(DateConstructorTest, StringTestWrong) {
  {
    std::istringstream is("+2017-01-01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("-2017-01-01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017*01-01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-01*01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017*01*01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017 01 01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-01 01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017 01-01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-%01-01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-01-%01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-01-%01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-/01-/01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-+01-+01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-01-+01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-+01-01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }

}

TEST(DateConstructorTest, StringTestWrong1) {
  {
    std::istringstream is("2017-01");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-01-");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
}

TEST(DateConstructorTest, StringTestWrong2) {
  {
    std::istringstream is("2017--1-1");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-1--1");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-13-1");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-1-32");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017--1--1");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017--15--15");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-15-55");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-0-55");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-02-0");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("2017-0-0");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
  {
    std::istringstream is("0-0-0");
    EXPECT_THROW(Date date(is.str()), std::runtime_error);
  }
}

TEST(DateConstructorTest, IntTestGood) {
  {
    Date date(2017, 02, 02);
    EXPECT_EQ(date.GetDate(), "2017-02-02");
  }
  {
    Date date(2017, 1, 9);
    EXPECT_EQ(date.GetDate(), "2017-01-09");
  }
  {
    Date date(2017, 9, 1);
    EXPECT_EQ(date.GetDate(), "2017-09-01");
  }
  {
    Date date(0, 01, 01);
    EXPECT_EQ(date.GetDate(), "0000-01-01");
  }
  {
    Date date(9999, 01, 01);
    EXPECT_EQ(date.GetDate(), "9999-01-01");
  }
  {
    Date date(11111, 01, 01);
    EXPECT_EQ(date.GetDate(), "11111-01-01");
  }
  {
    Date date(2019, 6, 15);
    EXPECT_EQ(date.GetDate(), "2019-06-15");
  }
  {
    Date date(2019, 12, 15);
    EXPECT_EQ(date.GetDate(), "2019-12-15");
  }
  {
    Date date(2019, 2, 31);
    EXPECT_EQ(date.GetDate(), "2019-02-31");
  }
  {
    Date date(2019, 12, 31);
    EXPECT_EQ(date.GetDate(), "2019-12-31");
  }
}

TEST(DateConstructorTest, IntTestWrong) {
  {
    EXPECT_THROW(Date date(-2017, 02, 02), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(2017, -02, 02), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(2017, 02, -02), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(2017, -02, -02), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(-2017, -02, -02), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(2017, 13, 02), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(2017, 02, 32), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(2017, 13, 32), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(2017, 0, 32), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(2017, 02, 0), std::runtime_error);
  }
  {
    EXPECT_THROW(Date date(2017, 0, 0), std::runtime_error);
  }
}

TEST(DateGetTest, YearGet) {
  {
    std::istringstream is("2019-1-1");
    Date date(is.str());
    EXPECT_EQ(date.GetYear(), 2019);
  }
  {
    std::istringstream is("0-1-1");
    Date date(is.str());
    EXPECT_EQ(date.GetYear(), 0);
  }
  {
    std::istringstream is("0000-1-1");
    Date date(is.str());
    EXPECT_EQ(date.GetYear(), 0);
  }
  {
    std::istringstream is("500-1-1");
    Date date(is.str());
    EXPECT_EQ(date.GetYear(), 500);
  }
  {
    std::istringstream is("41000-1-1");
    Date date(is.str());
    EXPECT_EQ(date.GetYear(), 41000);
  }
}

TEST(DateGetTest, MonthGet) {
  {
    std::istringstream is("2019-1-1");
    Date date(is.str());
    EXPECT_EQ(date.GetMonth(), 1);
  }
  {
    std::istringstream is("2019-6-1");
    Date date(is.str());
    EXPECT_EQ(date.GetMonth(), 6);
  }
  {
    std::istringstream is("2019-12-1");
    Date date(is.str());
    EXPECT_EQ(date.GetMonth(), 12);
  }
}

TEST(DateGetTest, DayGet) {
  {
    std::istringstream is("2019-1-1");
    Date date(is.str());
    EXPECT_EQ(date.GetDay(), 1);
  }
  {
    std::istringstream is("2019-1-15");
    Date date(is.str());
    EXPECT_EQ(date.GetDay(), 15);
  }
  {
    std::istringstream is("2019-1-31");
    Date date(is.str());
    EXPECT_EQ(date.GetDay(), 31);
  }
}

TEST(DateAssignTest, DayGet) {
  {
    std::istringstream is("2019-1-1");
    Date date1(is.str());
    Date date = date1;
    EXPECT_EQ(date.GetDate(), "2019-01-01");
  }
  {
    std::istringstream is("2019-6-12");
    Date date1(is.str());
    Date date = date1;
    EXPECT_EQ(date.GetDate(), "2019-06-12");
  }
  {
    std::istringstream is("2019-12-12");
    Date date1(is.str());
    Date date = date1;
    EXPECT_EQ(date.GetDate(), "2019-12-12");
  }
  {
    std::istringstream is("2019-12-31");
    Date date1(is.str());
    Date date = date1;
    EXPECT_EQ(date.GetDate(), "2019-12-31");
  }
}

TEST(DateCompareTest, EQ) {
  {
    Date date("2019-01-01");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() == date1.GetDate());
  }
  {
    Date date("2019-06-06");
    Date date1("2019-6-6");
    EXPECT_TRUE(date.GetDate() == date1.GetDate());
  }
  {
    Date date("2019-12-06");
    Date date1("2019-12-6");
    EXPECT_TRUE(date.GetDate() == date1.GetDate());
  }
  {
    Date date("2019-12-31");
    Date date1("2019-12-31");
    EXPECT_TRUE(date.GetDate() == date1.GetDate());
  }
  {
    Date date("2019-12-31");
    Date date1("2019-12-30");
    EXPECT_FALSE(date.GetDate() == date1.GetDate());
  }
}

TEST(DateCompareTest, NQ) {
  {
    Date date("2019-01-02");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() != date1.GetDate());
  }
  {
    Date date("2019-06-07");
    Date date1("2019-6-6");
    EXPECT_TRUE(date.GetDate() != date1.GetDate());
  }
  {
    Date date("2019-11-06");
    Date date1("2019-12-6");
    EXPECT_TRUE(date.GetDate() != date1.GetDate());
  }
  {
    Date date("2019-12-30");
    Date date1("2019-12-31");
    EXPECT_TRUE(date.GetDate() != date1.GetDate());
  }
  {
    Date date("2019-12-30");
    Date date1("2019-12-30");
    EXPECT_FALSE(date.GetDate() != date1.GetDate());
  }
}

TEST(DateCompareTest, LESS) {
  {
    Date date("2019-1-1");
    Date date1("2019-1-2");
    EXPECT_TRUE(date.GetDate() < date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2019-2-1");
    EXPECT_TRUE(date.GetDate() < date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-1-1");
    EXPECT_TRUE(date.GetDate() < date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-2-1");
    EXPECT_TRUE(date.GetDate() < date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-2-2");
    EXPECT_TRUE(date.GetDate() < date1.GetDate());
  }

  {
    Date date("2020-1-1");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() < date1.GetDate());
  }
  {
    Date date("2019-2-1");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() < date1.GetDate());
  }
  {
    Date date("2019-1-2");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() < date1.GetDate());
  }
  {
    Date date("2019-2-2");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() < date1.GetDate());
  }
  {
    Date date("2020-2-2");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() < date1.GetDate());
  }
}

TEST(DateCompareTest, LESS_EQUAL) {
  {
    Date date("2019-1-1");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() <= date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2019-1-2");
    EXPECT_TRUE(date.GetDate() <= date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2019-2-1");
    EXPECT_TRUE(date.GetDate() <= date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-1-1");
    EXPECT_TRUE(date.GetDate() <= date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-2-1");
    EXPECT_TRUE(date.GetDate() <= date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-2-2");
    EXPECT_TRUE(date.GetDate() <= date1.GetDate());
  }

  {
    Date date("2019-1-2");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() <= date1.GetDate());
  }
  {
    Date date("2019-2-1");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() <= date1.GetDate());
  }
  {
    Date date("2020-1-1");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() <= date1.GetDate());
  }
  {
    Date date("2020-2-1");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() <= date1.GetDate());
  }
  {
    Date date("2020-2-2");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() <= date1.GetDate());
  }
}

TEST(DateCompareTest, GREAT) {
  {
    Date date("2019-1-2");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() > date1.GetDate());
  }
  {
    Date date("2019-2-1");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() > date1.GetDate());
  }
  {
    Date date("2020-1-1");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() > date1.GetDate());
  }
  {
    Date date("2020-2-1");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() > date1.GetDate());
  }
  {
    Date date("2020-2-2");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() > date1.GetDate());
  }

  {
    Date date("2019-1-1");
    Date date1("2019-1-1");
    EXPECT_FALSE(date.GetDate() > date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2019-1-2");
    EXPECT_FALSE(date.GetDate() > date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2019-2-1");
    EXPECT_FALSE(date.GetDate() > date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-1-1");
    EXPECT_FALSE(date.GetDate() > date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-2-1");
    EXPECT_FALSE(date.GetDate() > date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-2-2");
    EXPECT_FALSE(date.GetDate() > date1.GetDate());
  }
}

TEST(DateCompareTest, GREAT_EQUAL) {
  {
    Date date("2019-1-1");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() >= date1.GetDate());
  }
  {
    Date date("2019-1-2");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() >= date1.GetDate());
  }
  {
    Date date("2019-2-1");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() >= date1.GetDate());
  }
  {
    Date date("2020-1-1");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() >= date1.GetDate());
  }
  {
    Date date("2020-2-1");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() >= date1.GetDate());
  }
  {
    Date date("2020-2-2");
    Date date1("2019-1-1");
    EXPECT_TRUE(date.GetDate() >= date1.GetDate());
  }

  {
    Date date("2019-1-1");
    Date date1("2019-1-2");
    EXPECT_FALSE(date.GetDate() >= date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2019-2-1");
    EXPECT_FALSE(date.GetDate() >= date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-1-1");
    EXPECT_FALSE(date.GetDate() >= date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-2-1");
    EXPECT_FALSE(date.GetDate() >= date1.GetDate());
  }
  {
    Date date("2019-1-1");
    Date date1("2020-2-2");
    EXPECT_FALSE(date.GetDate() >= date1.GetDate());
  }
}