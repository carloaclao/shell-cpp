#include "../utilities.h"

#include "gtest/gtest.h"

namespace {
  TEST(SplitTest, SplitsOnEverySeparatorOccurrence) {
    std::string test = "testing 1234 hello world";
    std::vector<std::string> res = split(' ', test);
    ASSERT_EQ(res.size(), 4);
    EXPECT_EQ(res.at(0), "testing");
    EXPECT_EQ(res.at(1), "1234");
    EXPECT_EQ(res.at(2), "hello");
  }

  TEST(SplitTest, EmptyInputYeldsSingleEmptyToken) {
    std::string test = "";
    std::vector<std::string> res = split(' ', test);
    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res.at(0), "");
  }

  TEST(SplitTest, NoSeparatorInInputAndInputIsResult) {
    std::string test = "Hello";
    std::vector<std::string> res = split(' ', test);
    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res.at(0), "Hello");
  }

  TEST(SplitTest, ConsecutiveSeparator) {
    std::string test = "Hello,,testing";
    std::vector<std::string> res = split(',', test);
    ASSERT_EQ(res.size(), 3);
    EXPECT_EQ(res.at(2), "testing");
  }
}
