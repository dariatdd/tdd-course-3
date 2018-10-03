/*
Given a year, report if it is a leap year.

The tricky thing here is that a leap year in the Gregorian calendar occurs:

on every year that is evenly divisible by 4
  except every year that is evenly divisible by 100
    unless the year is also evenly divisible by 400

For example, 1997 is not a leap year, but 1996 is. 1900 is not a leap year, but 2000 is.

If your language provides a method in the standard library that does this look-up, pretend it doesn't exist and implement it yourself.
*/

#include <gtest/gtest.h>


bool IsLeapYear(const int year)
{
    return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
}

TEST(LeapYear, DivisibleBy4)
{
    ASSERT_TRUE(IsLeapYear(1996));
}

TEST(LeapYear, DivisibleBy100)
{
    ASSERT_FALSE(IsLeapYear(1900));
}

TEST(LeapYear, DivisibleBy400)
{
    ASSERT_TRUE(IsLeapYear(2000));
}

TEST(LeapYear, NegativeYear)
{
    ASSERT_TRUE(IsLeapYear(-2000));
}

TEST(LealYear, ZeroYear)
{
    ASSERT_TRUE(IsLeapYear(0));
}
