#pragma once

#include <string>
#include <json/json.h>
#include "student_attendance/utils/AttendanceStatus.h"

namespace student_attendance
{
namespace models
{

struct Attendance
{
    int id;
    std::string studentId;
    std::string name;
    std::string className;
    std::string date;  // MM-DD format
    std::string status;
    std::string remark;

    Attendance() : id(0) {}

    Attendance(int i, const std::string &sid, const std::string &n,
               const std::string &cls, const std::string &d,
               const std::string &s, const std::string &r = "")
        : id(i), studentId(sid), name(n), className(cls),
          date(d), status(s), remark(r)
    {
    }

    Json::Value toJson() const
    {
        Json::Value json;
        json["id"] = id;
        json["student_id"] = studentId;
        json["name"] = name;
        json["class"] = className;
        json["date"] = date;
        json["status"] = status;
        json["status_symbol"] = utils::AttendanceStatus::getSymbol(status);
        json["remark"] = remark;
        return json;
    }

    static Attendance fromJson(const Json::Value &json)
    {
        Attendance att;
        if (json.isMember("id"))
            att.id = json["id"].asInt();
        if (json.isMember("student_id"))
            att.studentId = json["student_id"].asString();
        if (json.isMember("name"))
            att.name = json["name"].asString();
        if (json.isMember("class"))
            att.className = json["class"].asString();
        if (json.isMember("date"))
            att.date = json["date"].asString();
        if (json.isMember("status"))
            att.status = json["status"].asString();
        if (json.isMember("remark"))
            att.remark = json["remark"].asString();
        return att;
    }
};

}  // namespace models
}  // namespace student_attendance

