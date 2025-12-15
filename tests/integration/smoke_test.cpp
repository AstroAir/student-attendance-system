#include "project_name/core/greeting.hpp"

#include <gtest/gtest.h>

TEST(Integration, Smoke)
{
  const auto msg = project_name::core::greeting("integration");
  EXPECT_FALSE(msg.empty());
}
