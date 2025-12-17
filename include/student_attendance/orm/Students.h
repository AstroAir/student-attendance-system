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

class Students
{
public:
    static const std::string tableName;
    static const std::string primaryKeyName;
    using PrimaryKeyType = std::string;

    Students() = default;
    explicit Students(const drogon::orm::Row &row);

    // Getters
    const std::string &getStudentId() const noexcept { return studentId_; }
    const std::string &getName() const noexcept { return name_; }
    const std::string &getClassName() const noexcept { return className_; }
    const trantor::Date &getCreatedAt() const noexcept { return createdAt_; }
    const trantor::Date &getUpdatedAt() const noexcept { return updatedAt_; }

    // Setters
    void setStudentId(const std::string &id) { studentId_ = id; }
    void setName(const std::string &name) { name_ = name; }
    void setClassName(const std::string &className) { className_ = className; }
    void setCreatedAt(const trantor::Date &dt) { createdAt_ = dt; }
    void setUpdatedAt(const trantor::Date &dt) { updatedAt_ = dt; }

    // JSON conversion
    Json::Value toJson() const;
    Json::Value toBasicJson() const;
    static Students fromJson(const Json::Value &json);

    // Primary key
    const PrimaryKeyType &getPrimaryKey() const { return studentId_; }

    // For ORM mapper
    static const std::vector<std::string> &insertColumns();
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;

private:
    std::string studentId_;
    std::string name_;
    std::string className_;
    trantor::Date createdAt_;
    trantor::Date updatedAt_;
};

}  // namespace orm
}  // namespace student_attendance
