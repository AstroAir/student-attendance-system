#include "project_name/utils/string_utils.hpp"

#include <gtest/gtest.h>

TEST(StringUtils, ToUpper) { EXPECT_EQ(project_name::utils::to_upper("aBc"), "ABC"); }

TEST(StringUtils, Trim) { EXPECT_EQ(project_name::utils::trim("  hello \t"), "hello"); }

TEST(StringUtils, Split)
{
  const auto tokens = project_name::utils::split("a,b,c", ',');
  ASSERT_EQ(tokens.size(), 3U);
  EXPECT_EQ(tokens[0], "a");
  EXPECT_EQ(tokens[1], "b");
  EXPECT_EQ(tokens[2], "c");
}
