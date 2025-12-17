#include "student_attendance/services/AuthService.h"

#include "student_attendance/db/DatabaseManager.h"

#include <drogon/utils/Utilities.h>

namespace student_attendance
{
namespace services
{

std::optional<models::User> AuthService::authenticate(
    const std::string &username,
    const std::string &password) const
{
    if (username.empty() || password.empty())
    {
        return std::nullopt;
    }

    auto client = db::DatabaseManager::getInstance().getClient();
    if (!client)
    {
        return std::nullopt;
    }

    try
    {
        auto r = client->execSqlSync(
            "SELECT id, username, role, password_hash, salt FROM users WHERE username = ? LIMIT 1",
            username);
        if (r.empty())
        {
            return std::nullopt;
        }

        auto salt = r[0]["salt"].as<std::string>();
        auto expectedHash = r[0]["password_hash"].as<std::string>();
        auto actualHash = drogon::utils::getSha256(salt + password);

        if (expectedHash != actualHash)
        {
            return std::nullopt;
        }

        models::User user;
        user.id = r[0]["id"].as<int>();
        user.username = r[0]["username"].as<std::string>();
        user.role = r[0]["role"].as<std::string>();
        return user;
    }
    catch (const std::exception &)
    {
        return std::nullopt;
    }
}

}  // namespace services
}  // namespace student_attendance
