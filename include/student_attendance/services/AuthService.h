#pragma once

#include <optional>
#include <string>

#include "student_attendance/models/User.h"

namespace student_attendance
{
namespace services
{

class AuthService
{
public:
    static AuthService &getInstance()
    {
        static AuthService instance;
        return instance;
    }

    std::optional<models::User> authenticate(const std::string &username,
                                            const std::string &password) const;

private:
    AuthService() = default;
    ~AuthService() = default;
    AuthService(const AuthService &) = delete;
    AuthService &operator=(const AuthService &) = delete;
};

}  // namespace services
}  // namespace student_attendance
