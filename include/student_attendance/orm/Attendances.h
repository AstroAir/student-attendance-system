#pragma once

#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/Criteria.h>
#include <trantor/utils/Date.h>
#include <json/json.h>
#include <string>
#include <memory>
#include <optional>

namespace student_attendance
{
namespace orm
{

class Attendances
{
public:
    static const std::string tableName;
    static const std::string primaryKeyName;
    using PrimaryKeyType = int64_t;

    Attendances() = default;
    explicit Attendances(const drogon::orm::Row &row);

    // Getters
    int64_t getId() const noexcept { return id_; }
    const std::string &getStudentId() const noexcept { return studentId_; }
    const std::string &getDate() const noexcept { return date_; }
    const std::string &getStatus() const noexcept { return status_; }
    const std::string &getRemark() const noexcept { return remark_; }
    const trantor::Date &getCreatedAt() const noexcept { return createdAt_; }
    const trantor::Date &getUpdatedAt() const noexcept { return updatedAt_; }

    // Setters
    void setId(int64_t id) { id_ = id; }
    void setStudentId(const std::string &id) { studentId_ = id; }
    void setDate(const std::string &date) { date_ = date; }
    void setStatus(const std::string &status) { status_ = status; }
    void setRemark(const std::string &remark) { remark_ = remark; }
    void setCreatedAt(const trantor::Date &dt) { createdAt_ = dt; }
    void setUpdatedAt(const trantor::Date &dt) { updatedAt_ = dt; }

    // JSON conversion
    Json::Value toJson() const;
    static Attendances fromJson(const Json::Value &json);

    // Primary key
    const PrimaryKeyType &getPrimaryKey() const { return id_; }

    // For ORM mapper
    static const std::vector<std::string> &insertColumns();
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;

private:
    int64_t id_{0};
    std::string studentId_;
    std::string date_;
    std::string status_;
    std::string remark_;
    trantor::Date createdAt_;
    trantor::Date updatedAt_;
};

}  // namespace orm
}  // namespace student_attendance
