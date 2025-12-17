#include <gtest/gtest.h>
#include "student_attendance/services/AuthService.h"
#include "student_attendance/models/User.h"

using namespace student_attendance::services;
using namespace student_attendance::models;

class AuthApiTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

// POST /api/v1/auth/login - 用户登录
TEST_F(AuthApiTest, Authenticate_ValidCredentials)
{
    auto userOpt = AuthService::getInstance().authenticate("admin", "admin123");
    EXPECT_TRUE(userOpt.has_value());
    if (userOpt.has_value())
    {
        EXPECT_EQ(userOpt->username, "admin");
        EXPECT_FALSE(userOpt->role.empty());
    }
}

TEST_F(AuthApiTest, Authenticate_InvalidUsername)
{
    auto userOpt = AuthService::getInstance().authenticate("nonexistent", "password");
    EXPECT_FALSE(userOpt.has_value());
}

TEST_F(AuthApiTest, Authenticate_InvalidPassword)
{
    auto userOpt = AuthService::getInstance().authenticate("admin", "wrongpassword");
    EXPECT_FALSE(userOpt.has_value());
}

TEST_F(AuthApiTest, Authenticate_EmptyUsername)
{
    auto userOpt = AuthService::getInstance().authenticate("", "password");
    EXPECT_FALSE(userOpt.has_value());
}

TEST_F(AuthApiTest, Authenticate_EmptyPassword)
{
    auto userOpt = AuthService::getInstance().authenticate("admin", "");
    EXPECT_FALSE(userOpt.has_value());
}

TEST_F(AuthApiTest, Authenticate_BothEmpty)
{
    auto userOpt = AuthService::getInstance().authenticate("", "");
    EXPECT_FALSE(userOpt.has_value());
}

TEST_F(AuthApiTest, Authenticate_UserHasValidId)
{
    auto userOpt = AuthService::getInstance().authenticate("admin", "admin123");
    EXPECT_TRUE(userOpt.has_value());
    if (userOpt.has_value())
    {
        EXPECT_GT(userOpt->id, 0);
    }
}

TEST_F(AuthApiTest, Authenticate_UserToJson)
{
    auto userOpt = AuthService::getInstance().authenticate("admin", "admin123");
    EXPECT_TRUE(userOpt.has_value());
    if (userOpt.has_value())
    {
        auto json = userOpt->toJson();
        EXPECT_TRUE(json.isMember("id"));
        EXPECT_TRUE(json.isMember("username"));
        EXPECT_TRUE(json.isMember("role"));
        EXPECT_EQ(json["username"].asString(), "admin");
    }
}

TEST_F(AuthApiTest, Authenticate_CaseSensitiveUsername)
{
    auto userOpt = AuthService::getInstance().authenticate("Admin", "admin123");
    // Username should be case-sensitive, so this should fail
    // (depending on implementation, adjust expectation if needed)
    EXPECT_FALSE(userOpt.has_value());
}

TEST_F(AuthApiTest, Authenticate_WhitespaceInCredentials)
{
    auto userOpt = AuthService::getInstance().authenticate(" admin", "admin123");
    EXPECT_FALSE(userOpt.has_value());

    userOpt = AuthService::getInstance().authenticate("admin ", "admin123");
    EXPECT_FALSE(userOpt.has_value());
}

