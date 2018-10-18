/*### Bank OCR

Your manager has recently purchased a machine that assists in reading letters and faxes sent in by branch offices. The machine scans the paper documents, and produces a file with a number of entries. You will write a program to parse this file.

#### Specification
#### User Story 1

The following format is created by the machine:
```
    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|
```
Each entry is 3 lines long, and each line has 27 characters. 3 lines of each entry contain an account number written using pipes and underscores.

Each account number should have 9 digits, all of which should be in the range 0-9. A normal file contains around 500 entries.

Write a program that can take this file and parse it into actual account numbers.

Example input and output
```
 _  _  _  _  _  _  _  _  _
| || || || || || || || || |
|_||_||_||_||_||_||_||_||_|

=> 000000000

  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 111111111

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
|_ |_ |_ |_ |_ |_ |_ |_ |_

=> 222222222

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
 _| _| _| _| _| _| _| _| _|

=> 333333333

|_||_||_||_||_||_||_||_||_|
  |  |  |  |  |  |  |  |  |

=> 444444444

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
 _| _| _| _| _| _| _| _| _|

=> 555555555

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
|_||_||_||_||_||_||_||_||_|

=> 666666666

 _  _  _  _  _  _  _  _  _
  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 777777777

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
|_||_||_||_||_||_||_||_||_|

=> 888888888

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
 _| _| _| _| _| _| _| _| _|

=> 999999999

    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|

=> 123456789
```
*/
#include <gtest/gtest.h>
#include <string>

const unsigned short g_digitLen = 3;
const unsigned short g_linesInDigit = 3;
struct Digit
{
    std::vector<std::string> lines;
    unsigned short number;
};

const unsigned short g_digitsOnDisplay = 9;
struct Display
{
    std::vector<std::string> lines;
};

const Digit s_digit0 = {{ " _ ",
                          "| |",
                          "|_|"
                           },
                       0};
const Digit s_digit1 = {{ "   ",
                          "  |",
                          "  |"
                       },
                       1};
const Digit s_digit2 = {{ " _ ",
                          " _|",
                          "|_ "
                       },
                       2};
const Digit s_digit3 = {{ " _ ",
                          " _|",
                          " _|"
                       },
                       3};
const Digit s_digit4 = {{ "   ",
                          "|_|",
                          "  |"
                       },
                       4};
const Digit s_digit5 = {{ " _ ",
                          "|_ ",
                          " _|"
                       },
                       5};
const Digit s_digit6 = {{ " _ ",
                          "|_ ",
                          "|_|"
                       },
                       6};
const Digit s_digit7 = {{ " _ ",
                          "  |",
                          "  |"
                       },
                       7};
const Digit s_digit8 = {{ " _ ",
                          "|_|",
                          "|_|"
                       },
                       8};
const Digit s_digit9 = {{ " _ ",
                          "|_|",
                          " _|"
                       },
                       9};

const Display s_displayAll0 = {{ " _  _  _  _  _  _  _  _  _ ",
                                 "| || || || || || || || || |",
                                 "|_||_||_||_||_||_||_||_||_|"
}};

const Display s_displayAll1 = {{ "                           ",
                                 "  |  |  |  |  |  |  |  |  |",
                                 "  |  |  |  |  |  |  |  |  |"
}};

const Display s_displayAll2 = {{  " _  _  _  _  _  _  _  _  _ ",
                                  " _| _| _| _| _| _| _| _| _|",
                                  "|_ |_ |_ |_ |_ |_ |_ |_ |_ "
}};

const Display s_displayAll3 = {{ " _  _  _  _  _  _  _  _  _ ",
                                 " _| _| _| _| _| _| _| _| _|",
                                 " _| _| _| _| _| _| _| _| _|"
}};

const Display s_displayAll4 = {{ "                           ",
                                 "|_||_||_||_||_||_||_||_||_|",
                                 "  |  |  |  |  |  |  |  |  |"
}};

const Display s_displayAll5 = {{ " _  _  _  _  _  _  _  _  _ ",
                                 "|_ |_ |_ |_ |_ |_ |_ |_ |_ ",
                                 " _| _| _| _| _| _| _| _| _|"
}};

const Display s_displayAll6 = {{ " _  _  _  _  _  _  _  _  _ ",
                                 "|_ |_ |_ |_ |_ |_ |_ |_ |_ ",
                                 "|_||_||_||_||_||_||_||_||_|"
}};

const Display s_displayAll7 = {{ " _  _  _  _  _  _  _  _  _ ",
                                 "  |  |  |  |  |  |  |  |  |",
                                 "  |  |  |  |  |  |  |  |  |"
}};

const Display s_displayAll8 = {{ " _  _  _  _  _  _  _  _  _ ",
                                 "|_||_||_||_||_||_||_||_||_|",
                                 "|_||_||_||_||_||_||_||_||_|"
}};

const Display s_displayAll9 = {{ " _  _  _  _  _  _  _  _  _ ",
                                 "|_||_||_||_||_||_||_||_||_|",
                                 " _| _| _| _| _| _| _| _| _|"
}};

const Display s_display123456789 = {{ "    _  _     _  _  _  _  _ ",
                                      "  | _| _||_||_ |_   ||_||_|",
                                      "  ||_  _|  | _||_|  ||_| _|"
}};

const std::vector<Digit> s_allDigits = {s_digit0, s_digit1, s_digit2, s_digit3, s_digit4, s_digit5, s_digit6, s_digit7, s_digit8, s_digit9};

bool CheckDigitLine(const std::string& line, const std::string& lineToCompare)
{
    if(line.empty() || line.length() != g_digitLen)
    {
        throw std::exception("Invalid format");
    }

    return line.compare(lineToCompare) == 0;
}

unsigned short ConvertDigit(const Digit& digit)
{
    if(digit.lines.empty() || digit.lines.size() != g_linesInDigit)
    {
        throw std::exception("Invalid format");
    }

    for(const auto& pattern: s_allDigits)
    {
        bool matched = true;
        for(int i = 0; i < g_linesInDigit; ++i)
        {
            if(!CheckDigitLine(digit.lines[0], pattern.lines[0]))
            {
                matched = false;
                break;
            }
        }
        if(matched)
        {
            return pattern.number;
        }
    }

    throw std::exception("Invalid digit");
}

TEST(CheckDigitLine, EmptyString)
{
    EXPECT_THROW(CheckDigitLine("", ""), std::exception);
}

TEST(CheckDigitLine, LessThan3CharsString)
{
    EXPECT_THROW(CheckDigitLine("__", "__"), std::exception);
}

TEST(CheckDigitLine, ValidString)
{
    EXPECT_TRUE(CheckDigitLine("_ _", "_ _"));
}

TEST(CheckDigitLine, NotEqualStrings)
{
    EXPECT_FALSE(CheckDigitLine("_ _", "___"));
}

TEST(ConvertDigit, EmptyDigit)
{
    Digit g;
    EXPECT_THROW(ConvertDigit(g), std::exception);
}

TEST(ConvertDigit, InvalidNumberOfLinesDigit)
{
    Digit g = {{ "___",
                "_ _"
              }};
    EXPECT_THROW(ConvertDigit(g), std::exception);
}

TEST(ConvertDigit, Convert0Digit)
{
    EXPECT_EQ(ConvertDigit(s_digit0), 0);
}

TEST(ConvertDigit, Convert1Digit)
{
    EXPECT_EQ(ConvertDigit(s_digit1), 1);
}

TEST(ConvertDigit, ConvertInvalidDigit)
{
    Digit g = {{ "___",
                "_ _"
                "___"
              }, 0};
    EXPECT_THROW(ConvertDigit(g), std::exception);
}

TEST(ConvertDisplay, ConvertEmptyDisplay)
{
    Display d;
    EXPECT_THROW(ConvertDisplay(d), std::exception);
}
