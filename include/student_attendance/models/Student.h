#pragma once

#include <string>
#include <json/json.h>

namespace student_attendance
{
namespace models
{

struct Student
{
    std::string studentId;
    std::string name;
    std::string className;

    Student() = default;

    Student(const std::string &id, const std::string &n, const std::string &cls)
        : studentId(id), name(n), className(cls)
    {
    }

    Json::Value toJson() const
    {
        Json::Value json;
        json["student_id"] = studentId;
        json["name"] = name;
        json["class"] = className;
        return json;
    }

    Json::Value toBasicJson() const
    {
        Json::Value json;
        json["student_id"] = studentId;
        json["name"] = name;
        return json;
    }

    static Student fromJson(const Json::Value &json)
    {
        Student student;
        if (json.isMember("student_id"))
            student.studentId = json["student_id"].asString();
        if (json.isMember("name"))
            student.name = json["name"].asString();
        if (json.isMember("class"))
            student.className = json["class"].asString();
        return student;
    }
};

}  // namespace models
}  // namespace student_attendance

