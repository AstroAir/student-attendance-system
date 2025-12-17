#include <gtest/gtest.h>
#include "student_attendance/models/Student.h"
#include "student_attendance/models/Attendance.h"
#include "student_attendance/models/User.h"

using namespace student_attendance::models;

// ==================== Student Model Tests ====================

class StudentModelTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(StudentModelTest, DefaultConstructor)
{
    Student student;
    EXPECT_TRUE(student.studentId.empty());
    EXPECT_TRUE(student.name.empty());
    EXPECT_TRUE(student.className.empty());
}

TEST_F(StudentModelTest, ParameterizedConstructor)
{
    Student student("2024001", "张三", "人文2401班");
    EXPECT_EQ(student.studentId, "2024001");
    EXPECT_EQ(student.name, "张三");
    EXPECT_EQ(student.className, "人文2401班");
}

TEST_F(StudentModelTest, ToJson_ContainsAllFields)
{
    Student student("2024001", "张三", "人文2401班");
    auto json = student.toJson();

    EXPECT_TRUE(json.isMember("student_id"));
    EXPECT_TRUE(json.isMember("name"));
    EXPECT_TRUE(json.isMember("class"));

    EXPECT_EQ(json["student_id"].asString(), "2024001");
    EXPECT_EQ(json["name"].asString(), "张三");
    EXPECT_EQ(json["class"].asString(), "人文2401班");
}

TEST_F(StudentModelTest, ToBasicJson_ExcludesClass)
{
    Student student("2024001", "张三", "人文2401班");
    auto json = student.toBasicJson();

    EXPECT_TRUE(json.isMember("student_id"));
    EXPECT_TRUE(json.isMember("name"));
    EXPECT_FALSE(json.isMember("class"));

    EXPECT_EQ(json["student_id"].asString(), "2024001");
    EXPECT_EQ(json["name"].asString(), "张三");
}

TEST_F(StudentModelTest, FromJson_AllFields)
{
    Json::Value json;
    json["student_id"] = "2024002";
    json["name"] = "李四";
    json["class"] = "人文2402班";

    Student student = Student::fromJson(json);

    EXPECT_EQ(student.studentId, "2024002");
    EXPECT_EQ(student.name, "李四");
    EXPECT_EQ(student.className, "人文2402班");
}

TEST_F(StudentModelTest, FromJson_PartialFields)
{
    Json::Value json;
    json["student_id"] = "2024003";

    Student student = Student::fromJson(json);

    EXPECT_EQ(student.studentId, "2024003");
    EXPECT_TRUE(student.name.empty());
    EXPECT_TRUE(student.className.empty());
}

TEST_F(StudentModelTest, FromJson_EmptyJson)
{
    Json::Value json;
    Student student = Student::fromJson(json);

    EXPECT_TRUE(student.studentId.empty());
    EXPECT_TRUE(student.name.empty());
    EXPECT_TRUE(student.className.empty());
}

TEST_F(StudentModelTest, ToJsonFromJson_RoundTrip)
{
    Student original("2024001", "张三", "人文2401班");
    auto json = original.toJson();
    Student restored = Student::fromJson(json);

    EXPECT_EQ(original.studentId, restored.studentId);
    EXPECT_EQ(original.name, restored.name);
    EXPECT_EQ(original.className, restored.className);
}

TEST_F(StudentModelTest, SpecialCharactersInFields)
{
    Student student("2024-001", "张三/李四", "人文2401班（A）");
    auto json = student.toJson();

    EXPECT_EQ(json["student_id"].asString(), "2024-001");
    EXPECT_EQ(json["name"].asString(), "张三/李四");
    EXPECT_EQ(json["class"].asString(), "人文2401班（A）");
}

// ==================== Attendance Model Tests ====================

class AttendanceModelTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(AttendanceModelTest, DefaultConstructor)
{
    Attendance att;
    EXPECT_EQ(att.id, 0);
    EXPECT_TRUE(att.studentId.empty());
    EXPECT_TRUE(att.name.empty());
    EXPECT_TRUE(att.className.empty());
    EXPECT_TRUE(att.date.empty());
    EXPECT_TRUE(att.status.empty());
    EXPECT_TRUE(att.remark.empty());
}

TEST_F(AttendanceModelTest, ParameterizedConstructor)
{
    Attendance att(1, "2024001", "张三", "人文2401班", "12-15", "present", "正常出勤");

    EXPECT_EQ(att.id, 1);
    EXPECT_EQ(att.studentId, "2024001");
    EXPECT_EQ(att.name, "张三");
    EXPECT_EQ(att.className, "人文2401班");
    EXPECT_EQ(att.date, "12-15");
    EXPECT_EQ(att.status, "present");
    EXPECT_EQ(att.remark, "正常出勤");
}

TEST_F(AttendanceModelTest, ParameterizedConstructor_DefaultRemark)
{
    Attendance att(1, "2024001", "张三", "人文2401班", "12-15", "present");
    EXPECT_TRUE(att.remark.empty());
}

TEST_F(AttendanceModelTest, ToJson_ContainsAllFields)
{
    Attendance att(1, "2024001", "张三", "人文2401班", "12-15", "present", "备注");
    auto json = att.toJson();

    EXPECT_TRUE(json.isMember("id"));
    EXPECT_TRUE(json.isMember("student_id"));
    EXPECT_TRUE(json.isMember("name"));
    EXPECT_TRUE(json.isMember("class"));
    EXPECT_TRUE(json.isMember("date"));
    EXPECT_TRUE(json.isMember("status"));
    EXPECT_TRUE(json.isMember("status_symbol"));
    EXPECT_TRUE(json.isMember("remark"));

    EXPECT_EQ(json["id"].asInt(), 1);
    EXPECT_EQ(json["student_id"].asString(), "2024001");
    EXPECT_EQ(json["name"].asString(), "张三");
    EXPECT_EQ(json["class"].asString(), "人文2401班");
    EXPECT_EQ(json["date"].asString(), "12-15");
    EXPECT_EQ(json["status"].asString(), "present");
    EXPECT_EQ(json["status_symbol"].asString(), "√");
    EXPECT_EQ(json["remark"].asString(), "备注");
}

TEST_F(AttendanceModelTest, ToJson_StatusSymbols)
{
    std::vector<std::pair<std::string, std::string>> statusSymbols = {
        {"present", "√"},
        {"absent", "X"},
        {"personal_leave", "△"},
        {"sick_leave", "○"},
        {"late", "+"},
        {"early_leave", "–"}
    };

    for (const auto &[status, symbol] : statusSymbols)
    {
        Attendance att(1, "2024001", "张三", "人文2401班", "12-15", status);
        auto json = att.toJson();
        EXPECT_EQ(json["status_symbol"].asString(), symbol)
            << "Status: " << status << " should have symbol: " << symbol;
    }
}

TEST_F(AttendanceModelTest, FromJson_AllFields)
{
    Json::Value json;
    json["id"] = 5;
    json["student_id"] = "2024002";
    json["name"] = "李四";
    json["class"] = "人文2402班";
    json["date"] = "12-16";
    json["status"] = "late";
    json["remark"] = "迟到5分钟";

    Attendance att = Attendance::fromJson(json);

    EXPECT_EQ(att.id, 5);
    EXPECT_EQ(att.studentId, "2024002");
    EXPECT_EQ(att.name, "李四");
    EXPECT_EQ(att.className, "人文2402班");
    EXPECT_EQ(att.date, "12-16");
    EXPECT_EQ(att.status, "late");
    EXPECT_EQ(att.remark, "迟到5分钟");
}

TEST_F(AttendanceModelTest, FromJson_PartialFields)
{
    Json::Value json;
    json["id"] = 10;
    json["student_id"] = "2024003";

    Attendance att = Attendance::fromJson(json);

    EXPECT_EQ(att.id, 10);
    EXPECT_EQ(att.studentId, "2024003");
    EXPECT_TRUE(att.name.empty());
    EXPECT_TRUE(att.className.empty());
}

TEST_F(AttendanceModelTest, FromJson_EmptyJson)
{
    Json::Value json;
    Attendance att = Attendance::fromJson(json);

    EXPECT_EQ(att.id, 0);
    EXPECT_TRUE(att.studentId.empty());
}

TEST_F(AttendanceModelTest, ToJsonFromJson_RoundTrip)
{
    Attendance original(1, "2024001", "张三", "人文2401班", "12-15", "present", "备注");
    auto json = original.toJson();
    Attendance restored = Attendance::fromJson(json);

    EXPECT_EQ(original.id, restored.id);
    EXPECT_EQ(original.studentId, restored.studentId);
    EXPECT_EQ(original.name, restored.name);
    EXPECT_EQ(original.className, restored.className);
    EXPECT_EQ(original.date, restored.date);
    EXPECT_EQ(original.status, restored.status);
    EXPECT_EQ(original.remark, restored.remark);
}

// ==================== User Model Tests ====================

class UserModelTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(UserModelTest, DefaultValues)
{
    User user;
    EXPECT_EQ(user.id, 0);
    EXPECT_TRUE(user.username.empty());
    EXPECT_TRUE(user.role.empty());
}

TEST_F(UserModelTest, ToJson_ContainsAllFields)
{
    User user;
    user.id = 1;
    user.username = "admin";
    user.role = "administrator";

    auto json = user.toJson();

    EXPECT_TRUE(json.isMember("id"));
    EXPECT_TRUE(json.isMember("username"));
    EXPECT_TRUE(json.isMember("role"));

    EXPECT_EQ(json["id"].asInt(), 1);
    EXPECT_EQ(json["username"].asString(), "admin");
    EXPECT_EQ(json["role"].asString(), "administrator");
}

TEST_F(UserModelTest, ToJson_EmptyUser)
{
    User user;
    auto json = user.toJson();

    EXPECT_EQ(json["id"].asInt(), 0);
    EXPECT_EQ(json["username"].asString(), "");
    EXPECT_EQ(json["role"].asString(), "");
}

TEST_F(UserModelTest, FieldAssignment)
{
    User user;
    user.id = 100;
    user.username = "testuser";
    user.role = "teacher";

    EXPECT_EQ(user.id, 100);
    EXPECT_EQ(user.username, "testuser");
    EXPECT_EQ(user.role, "teacher");
}

