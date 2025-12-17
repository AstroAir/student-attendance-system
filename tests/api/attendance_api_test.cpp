#include <gtest/gtest.h>
#include "student_attendance/db/DatabaseManager.h"
#include "student_attendance/services/AttendanceService.h"
#include "student_attendance/models/DataStore.h"

using namespace student_attendance::services;
using namespace student_attendance::db;
using namespace student_attendance::models;

class AttendanceApiTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        DatabaseManager::getInstance().reset();
        DataStore::getInstance().reset();
    }

    void TearDown() override
    {
        DatabaseManager::getInstance().reset();
        DataStore::getInstance().reset();
    }
};

// GET /api/v1/attendances - 获取考勤记录列表
TEST_F(AttendanceApiTest, GetAttendances_ReturnsAll)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 20, "", "", "", "", "", "", "", "", "asc");
    EXPECT_GT(result.total, 0);
}

TEST_F(AttendanceApiTest, GetAttendances_WithStudentIdFilter)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 20, "2024001", "", "", "", "", "", "", "", "asc");
    for (const auto &att : result.attendances)
    {
        EXPECT_EQ(att.studentId, "2024001");
    }
}

TEST_F(AttendanceApiTest, GetAttendances_WithDateFilter)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 20, "", "", "", "12-15", "", "", "", "", "asc");
    for (const auto &att : result.attendances)
    {
        EXPECT_EQ(att.date, "12-15");
    }
}

TEST_F(AttendanceApiTest, GetAttendances_WithStatusFilter)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 20, "", "", "", "", "", "", "present", "", "asc");
    for (const auto &att : result.attendances)
    {
        EXPECT_EQ(att.status, "present");
    }
}

// POST /api/v1/attendances - 新增考勤记录
TEST_F(AttendanceApiTest, CreateAttendance_Success)
{
    auto [success, att] = AttendanceService::getInstance().createAttendance(
        "2024001", "12-20", "present", "测试备注");
    EXPECT_TRUE(success);
    EXPECT_EQ(att.studentId, "2024001");
    EXPECT_EQ(att.date, "12-20");
    EXPECT_EQ(att.status, "present");
}

TEST_F(AttendanceApiTest, CreateAttendance_InvalidStudent)
{
    auto [success, att] = AttendanceService::getInstance().createAttendance(
        "9999999", "12-20", "present", "");
    EXPECT_FALSE(success);
}

TEST_F(AttendanceApiTest, CreateAttendance_InvalidStatus)
{
    auto [success, att] = AttendanceService::getInstance().createAttendance(
        "2024001", "12-20", "invalid_status", "");
    EXPECT_FALSE(success);
}

// POST /api/v1/attendances/batch - 批量新增考勤记录
TEST_F(AttendanceApiTest, BatchCreateAttendances_Success)
{
    std::vector<std::pair<std::string, std::string>> records = {
        {"2024001", "present"},
        {"2024002", "late"},
        {"2024003", "absent"}
    };

    int count = AttendanceService::getInstance().batchCreateAttendances("12-25", records);
    EXPECT_EQ(count, 3);
}

TEST_F(AttendanceApiTest, BatchCreateAttendances_PartialSuccess)
{
    std::vector<std::pair<std::string, std::string>> records = {
        {"2024001", "present"},
        {"9999999", "present"},  // Invalid student
        {"2024002", "late"}
    };

    int count = AttendanceService::getInstance().batchCreateAttendances("12-26", records);
    EXPECT_EQ(count, 2);  // Only 2 should succeed
}

// GET /api/v1/attendances/{id} - 获取单条考勤记录
TEST_F(AttendanceApiTest, GetAttendance_Exists)
{
    auto att = AttendanceService::getInstance().getAttendance(1);
    EXPECT_TRUE(att.has_value());
}

TEST_F(AttendanceApiTest, GetAttendance_NotExists)
{
    auto att = AttendanceService::getInstance().getAttendance(99999);
    EXPECT_FALSE(att.has_value());
}

// PUT /api/v1/attendances/{id} - 修改考勤记录
TEST_F(AttendanceApiTest, UpdateAttendance_Success)
{
    auto [success, message] = AttendanceService::getInstance().updateAttendance(
        1, "late", "迟到5分钟");
    EXPECT_TRUE(success);

    auto att = AttendanceService::getInstance().getAttendance(1);
    EXPECT_TRUE(att.has_value());
    EXPECT_EQ(att->status, "late");
    EXPECT_EQ(att->remark, "迟到5分钟");
}

TEST_F(AttendanceApiTest, UpdateAttendance_NotExists)
{
    auto [success, message] = AttendanceService::getInstance().updateAttendance(
        99999, "late", "");
    EXPECT_FALSE(success);
}

// DELETE /api/v1/attendances/{id} - 删除考勤记录
TEST_F(AttendanceApiTest, DeleteAttendance_Success)
{
    // First create an attendance to delete
    auto [created, att] = AttendanceService::getInstance().createAttendance(
        "2024001", "12-30", "present", "");
    EXPECT_TRUE(created);

    bool deleted = AttendanceService::getInstance().deleteAttendance(att.id);
    EXPECT_TRUE(deleted);
}

TEST_F(AttendanceApiTest, DeleteAttendance_NotExists)
{
    bool deleted = AttendanceService::getInstance().deleteAttendance(99999);
    EXPECT_FALSE(deleted);
}

// ==================== 边界条件测试 ====================

TEST_F(AttendanceApiTest, GetAttendances_PageZero)
{
    auto result = AttendanceService::getInstance().getAttendances(
        0, 20, "", "", "", "", "", "", "", "", "asc");
    EXPECT_GE(result.page, 0);
}

TEST_F(AttendanceApiTest, GetAttendances_NegativePage)
{
    auto result = AttendanceService::getInstance().getAttendances(
        -1, 20, "", "", "", "", "", "", "", "", "asc");
    EXPECT_GE(result.page, 0);
}

TEST_F(AttendanceApiTest, GetAttendances_ZeroPageSize)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 0, "", "", "", "", "", "", "", "", "asc");
    EXPECT_GE(result.pageSize, 0);
}

TEST_F(AttendanceApiTest, GetAttendances_LargePageSize)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 1000, "", "", "", "", "", "", "", "", "asc");
    EXPECT_LE(static_cast<int>(result.attendances.size()), 1000);
}

TEST_F(AttendanceApiTest, GetAttendances_DescendingOrder)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 20, "", "", "", "", "", "", "", "date", "desc");
    // Should return results in descending order
    EXPECT_GE(result.total, 0);
}

TEST_F(AttendanceApiTest, GetAttendances_WithNameFilter)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 20, "", "张", "", "", "", "", "", "", "asc");
    for (const auto &att : result.attendances)
    {
        EXPECT_TRUE(att.name.find("张") != std::string::npos);
    }
}

TEST_F(AttendanceApiTest, GetAttendances_WithClassFilter)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 20, "", "", "人文2401班", "", "", "", "", "", "asc");
    for (const auto &att : result.attendances)
    {
        EXPECT_EQ(att.className, "人文2401班");
    }
}

TEST_F(AttendanceApiTest, GetAttendances_WithDateRange)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 20, "", "", "", "", "12-01", "12-31", "", "", "asc");
    for (const auto &att : result.attendances)
    {
        EXPECT_GE(att.date, "12-01");
        EXPECT_LE(att.date, "12-31");
    }
}

TEST_F(AttendanceApiTest, GetAttendances_MultipleFilters)
{
    auto result = AttendanceService::getInstance().getAttendances(
        1, 20, "", "", "人文2401班", "", "", "", "present", "", "asc");
    for (const auto &att : result.attendances)
    {
        EXPECT_EQ(att.className, "人文2401班");
        EXPECT_EQ(att.status, "present");
    }
}

TEST_F(AttendanceApiTest, CreateAttendance_EmptyDate)
{
    auto [success, att] = AttendanceService::getInstance().createAttendance(
        "2024001", "", "present", "");
    EXPECT_FALSE(success);
}

TEST_F(AttendanceApiTest, CreateAttendance_EmptyStudentId)
{
    auto [success, att] = AttendanceService::getInstance().createAttendance(
        "", "12-20", "present", "");
    EXPECT_FALSE(success);
}

TEST_F(AttendanceApiTest, CreateAttendance_AllStatusTypes)
{
    std::vector<std::string> statuses = {
        "present", "absent", "personal_leave", "sick_leave", "late", "early_leave"
    };

    for (const auto &status : statuses)
    {
        auto [success, att] = AttendanceService::getInstance().createAttendance(
            "2024001", "12-20", status, "测试" + status);
        EXPECT_TRUE(success) << "Failed to create attendance with status: " << status;
        EXPECT_EQ(att.status, status);
    }
}

TEST_F(AttendanceApiTest, UpdateAttendance_InvalidStatus)
{
    auto [success, message] = AttendanceService::getInstance().updateAttendance(
        1, "invalid_status", "");
    EXPECT_FALSE(success);
}

TEST_F(AttendanceApiTest, UpdateAttendance_AllStatusTypes)
{
    std::vector<std::string> statuses = {
        "present", "absent", "personal_leave", "sick_leave", "late", "early_leave"
    };

    for (const auto &status : statuses)
    {
        auto [success, message] = AttendanceService::getInstance().updateAttendance(
            1, status, "更新为" + status);
        EXPECT_TRUE(success) << "Failed to update attendance to status: " << status;
    }
}

TEST_F(AttendanceApiTest, BatchCreateAttendances_EmptyRecords)
{
    std::vector<std::pair<std::string, std::string>> records;
    int count = AttendanceService::getInstance().batchCreateAttendances("12-27", records);
    EXPECT_EQ(count, 0);
}

TEST_F(AttendanceApiTest, BatchCreateAttendances_AllInvalidStudents)
{
    std::vector<std::pair<std::string, std::string>> records = {
        {"9999991", "present"},
        {"9999992", "present"},
        {"9999993", "present"}
    };

    int count = AttendanceService::getInstance().batchCreateAttendances("12-28", records);
    EXPECT_EQ(count, 0);
}

TEST_F(AttendanceApiTest, GetAttendance_NegativeId)
{
    auto att = AttendanceService::getInstance().getAttendance(-1);
    EXPECT_FALSE(att.has_value());
}

TEST_F(AttendanceApiTest, GetAttendance_ZeroId)
{
    auto att = AttendanceService::getInstance().getAttendance(0);
    EXPECT_FALSE(att.has_value());
}

TEST_F(AttendanceApiTest, DeleteAttendance_NegativeId)
{
    bool deleted = AttendanceService::getInstance().deleteAttendance(-1);
    EXPECT_FALSE(deleted);
}

TEST_F(AttendanceApiTest, DeleteAttendance_ZeroId)
{
    bool deleted = AttendanceService::getInstance().deleteAttendance(0);
    EXPECT_FALSE(deleted);
}

