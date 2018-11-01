/*
Write a function, that is given a string and a length limit, splits provided string into sequence of string,
where length of each string is not more, than provided limit. If there are spaces under provided limit -
last space should be used to wrapp this line. If there are no spaces - wrapp it on provided length limit.

Example:
When pos is specified, the search only includes sequences of characters that begin at or before position pos,
ignoring any possible match beginning after pos

"When pos is specified, the",
"search only includes sequences",
"of characters that begin at or",
"before position pos, ignoring",
"any possible match beginning",
"after pos."
*/

#include <gtest/gtest.h>
#include <cctype>


// empty string
// string shorter than wrap number
// word longer than wrap number
// word much longer than wrap number (more than 2 strings)
// string longer than wrap number
// string ends with one whitespace longer than limit


// string should be wrapped by space if it is present under limit (two words in string, space before limit)
// string should be wrapped by space if it is present under limit (three words in string, two spaces before limit)
// string should be wrapped by space if it is present under limit (three words in string, no space before limit)
// only whitespaces in string

using WrappedStrings = std::vector<std::string>;

void TrimString(std::string& str)
{
   str.erase(0, str.find_first_not_of(' '));
   str.erase(str.find_last_not_of(' ') + 1);
}

WrappedStrings WrapString(const std::string& str, size_t wrapLength)
{
    WrappedStrings result;
    size_t curLimit = wrapLength;
    for(size_t i = 0; i < str.length(); i += curLimit)
    {
        auto pos = str.find_last_of(' ', i + curLimit);
        if(pos != std::string::npos && (pos > i && pos < wrapLength))
        {
            curLimit = pos + 1;
        }
        else
        {
            curLimit = wrapLength;
        }

        std::string cur = str.substr(i, curLimit);
        TrimString(cur);

        if(!cur.empty() && cur != " ")
        {
            result.push_back(cur);
        }
    }

    return result;
}

TEST(WrapString, EmptyString)
{
    ASSERT_EQ(WrappedStrings(), WrapString("", 25));
}

TEST(WrapString, StringShorterWrapNumber)
{
    ASSERT_EQ(WrappedStrings{"asdf"}, WrapString("asdf", 8));
}

TEST(WrapString, StringLongerThanWrapNumber)
{
    WrappedStrings expected = {"asd", "f"};
    ASSERT_EQ(expected, WrapString("asdf", 3));
}

TEST(WrapString, StringLongerThanWrapNumberSeveralParts)
{
    WrappedStrings expected = {"12", "34", "56"};
    ASSERT_EQ(expected, WrapString("123456", 2));
}

TEST(WrapString, MultipleWordsLonger)
{
    WrappedStrings expected = {"1", "2"};
    ASSERT_EQ(expected, WrapString("1 2", 1));
}

TEST(WrapString, SpaceStringEnd)
{
    WrappedStrings expected = {"1", "2"};
    ASSERT_EQ(expected, WrapString("1 2", 2));
}

TEST(WrapString, StringWrappedBySeveralWhitespace)
{
    WrappedStrings expected = {"12", "34"};
    ASSERT_EQ(expected, WrapString("12  34", 3));
}

TEST(WrapString, StringEndsWithOneWhitespace)
{
    WrappedStrings expected = {"123"};
    ASSERT_EQ(expected, WrapString("123 ", 3));
}

TEST(WrapString, TwoWordsSpaceBeforeLimit)
{
    WrappedStrings expected = {"1", "234"};
    ASSERT_EQ(expected, WrapString("1 234", 3));
}

TEST(WrapString, ThreeWordsTwoSpaceBeforeLimit)
{
    WrappedStrings expected = {"1 2", "345"};
    ASSERT_EQ(expected, WrapString("1 2 345", 5));
}

TEST(WrapString, TwoWordsNoSpaceBeforeLimit)
{
    WrappedStrings expected = {"12", "3", "45"};
    ASSERT_EQ(expected, WrapString("123 45", 2));
}

TEST(WrapString, OnlySpaces)
{
    WrappedStrings expected = {};
    ASSERT_EQ(expected, WrapString("    ", 2));
}


TEST(WrapString, OnlySpacesThreeSpacesInBlock)
{
    WrappedStrings expected = {};
    ASSERT_EQ(expected, WrapString("      ", 3));
}
