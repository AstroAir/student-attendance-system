#include "project_name/core/greeting.hpp"

#include <gtest/gtest.h>

TEST(Greeting, EmptyName) { EXPECT_EQ(project_name::core::greeting(""), "Hello!"); }

TEST(Greeting, WithName) { EXPECT_EQ(project_name::core::greeting("Alice"), "Hello, Alice!"); }
