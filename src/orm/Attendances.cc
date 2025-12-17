#include "student_attendance/orm/Attendances.h"
#include "student_attendance/utils/AttendanceStatus.h"

namespace student_attendance
{
namespace orm
{

const std::string Attendances::tableName = "attendances";
const std::string Attendances::primaryKeyName = "id";

Attendances::Attendances(const drogon::orm::Row &row)
{
    if (!row["id"].isNull())
        id_ = row["id"].as<int64_t>();
    if (!row["student_id"].isNull())
        studentId_ = row["student_id"].as<std::string>();
    if (!row["date"].isNull())
        date_ = row["date"].as<std::string>();
    if (!row["status"].isNull())
        status_ = row["status"].as<std::string>();
    if (!row["remark"].isNull())
        remark_ = row["remark"].as<std::string>();
    if (!row["created_at"].isNull())
        createdAt_ = row["created_at"].as<trantor::Date>();
    if (!row["updated_at"].isNull())
        updatedAt_ = row["updated_at"].as<trantor::Date>();
}

Json::Value Attendances::toJson() const
{
    Json::Value json;
    json["id"] = static_cast<Json::Int64>(id_);
    json["student_id"] = studentId_;
    json["date"] = date_;
    json["status"] = status_;
    json["status_symbol"] = utils::AttendanceStatus::getSymbol(status_);
    json["remark"] = remark_;
    json["created_at"] = createdAt_.toDbStringLocal();
    json["updated_at"] = updatedAt_.toDbStringLocal();
    return json;
}

Attendances Attendances::fromJson(const Json::Value &json)
{
    Attendances att;
    if (json.isMember("id"))
        att.setId(json["id"].asInt64());
    if (json.isMember("student_id"))
        att.setStudentId(json["student_id"].asString());
    if (json.isMember("date"))
        att.setDate(json["date"].asString());
    if (json.isMember("status"))
        att.setStatus(json["status"].asString());
    if (json.isMember("remark"))
        att.setRemark(json["remark"].asString());
    return att;
}

const std::vector<std::string> &Attendances::insertColumns()
{
    static const std::vector<std::string> cols = {
        "student_id", "date", "status", "remark"
    };
    return cols;
}

void Attendances::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    binder << studentId_ << date_ << status_ << remark_;
}

void Attendances::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    binder << status_ << remark_ << id_;
}

}  // namespace orm
}  // namespace student_attendance
