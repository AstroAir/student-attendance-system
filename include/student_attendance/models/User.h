#pragma once

#include <string>
#include <json/json.h>

namespace student_attendance
{
namespace models
{

struct User
{
    int id{0};
    std::string username;
    std::string role;

    Json::Value toJson() const
    {
        Json::Value json;
        json["id"] = id;
        json["username"] = username;
        json["role"] = role;
        return json;
    }
};

}  // namespace models
}  // namespace student_attendance
