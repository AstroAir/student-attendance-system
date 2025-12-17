#include "student_attendance/orm/Students.h"

namespace student_attendance
{
namespace orm
{

const std::string Students::tableName = "students";
const std::string Students::primaryKeyName = "student_id";

Students::Students(const drogon::orm::Row &row)
{
    if (!row["student_id"].isNull())
        studentId_ = row["student_id"].as<std::string>();
    if (!row["name"].isNull())
        name_ = row["name"].as<std::string>();
    if (!row["class_name"].isNull())
        className_ = row["class_name"].as<std::string>();
    if (!row["created_at"].isNull())
        createdAt_ = row["created_at"].as<trantor::Date>();
    if (!row["updated_at"].isNull())
        updatedAt_ = row["updated_at"].as<trantor::Date>();
}

Json::Value Students::toJson() const
{
    Json::Value json;
    json["student_id"] = studentId_;
    json["name"] = name_;
    json["class"] = className_;
    json["created_at"] = createdAt_.toDbStringLocal();
    json["updated_at"] = updatedAt_.toDbStringLocal();
    return json;
}

Json::Value Students::toBasicJson() const
{
    Json::Value json;
    json["student_id"] = studentId_;
    json["name"] = name_;
    return json;
}

Students Students::fromJson(const Json::Value &json)
{
    Students student;
    if (json.isMember("student_id"))
        student.setStudentId(json["student_id"].asString());
    if (json.isMember("name"))
        student.setName(json["name"].asString());
    if (json.isMember("class"))
        student.setClassName(json["class"].asString());
    return student;
}

const std::vector<std::string> &Students::insertColumns()
{
    static const std::vector<std::string> cols = {
        "student_id", "name", "class_name"
    };
    return cols;
}

void Students::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    binder << studentId_ << name_ << className_;
}

void Students::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    binder << name_ << className_ << studentId_;
}

}  // namespace orm
}  // namespace student_attendance
