#include <gtest/gtest.h>
#include "student_attendance/db/DatabaseManager.h"
#include "student_attendance/services/ReportService.h"
#include "student_attendance/models/DataStore.h"

using namespace student_attendance::services;
using namespace student_attendance::db;
using namespace student_attendance::models;

class ReportApiTest : public ::testing::Test
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

// GET /api/v1/reports/details - 考勤明细表
TEST_F(ReportApiTest, GetDetailsReport_Success)
{
    auto data = ReportService::getInstance().getDetailsReport(
        "12-01", "12-31", "", "");
    EXPECT_TRUE(data.isMember("period"));
    EXPECT_TRUE(data.isMember("records"));
}

TEST_F(ReportApiTest, GetDetailsReport_WithClassFilter)
{
    auto data = ReportService::getInstance().getDetailsReport(
        "12-01", "12-31", "人文2401班", "");
    EXPECT_TRUE(data.isMember("records"));
}

TEST_F(ReportApiTest, GetDetailsReport_WithStudentFilter)
{
    auto data = ReportService::getInstance().getDetailsReport(
        "12-01", "12-31", "", "2024001");
    EXPECT_TRUE(data.isMember("records"));
}

// GET /api/v1/reports/daily - 考勤日报表
TEST_F(ReportApiTest, GetDailyReport_Success)
{
    auto data = ReportService::getInstance().getDailyReport("12-15", "");
    EXPECT_TRUE(data.isMember("date"));
    EXPECT_TRUE(data.isMember("summary"));
    EXPECT_TRUE(data.isMember("details"));
}

TEST_F(ReportApiTest, GetDailyReport_WithClassFilter)
{
    auto data = ReportService::getInstance().getDailyReport("12-15", "人文2401班");
    EXPECT_TRUE(data.isMember("details"));
}

// GET /api/v1/reports/summary - 考勤汇总表
TEST_F(ReportApiTest, GetSummaryReport_Success)
{
    auto data = ReportService::getInstance().getSummaryReport(
        "12-01", "12-31", "");
    EXPECT_TRUE(data.isMember("period"));
    EXPECT_TRUE(data.isMember("summary"));
}

TEST_F(ReportApiTest, GetSummaryReport_WithClassFilter)
{
    auto data = ReportService::getInstance().getSummaryReport(
        "12-01", "12-31", "人文2401班");
    EXPECT_TRUE(data.isMember("summary"));
}

// GET /api/v1/reports/abnormal - 考勤异常表
TEST_F(ReportApiTest, GetAbnormalReport_Success)
{
    auto data = ReportService::getInstance().getAbnormalReport(
        "12-01", "12-31", "", "");
    EXPECT_TRUE(data.isMember("period"));
    EXPECT_TRUE(data.isMember("abnormal_records"));
    EXPECT_TRUE(data.isMember("statistics"));
}

TEST_F(ReportApiTest, GetAbnormalReport_WithTypeFilter)
{
    auto data = ReportService::getInstance().getAbnormalReport(
        "12-01", "12-31", "", "absent");
    EXPECT_TRUE(data.isMember("abnormal_records"));
}

// GET /api/v1/reports/leave - 请假汇总表
TEST_F(ReportApiTest, GetLeaveReport_Success)
{
    auto data = ReportService::getInstance().getLeaveReport(
        "12-01", "12-31", "", "");
    EXPECT_TRUE(data.isMember("period"));
    EXPECT_TRUE(data.isMember("leave_records"));
    EXPECT_TRUE(data.isMember("statistics"));
}

TEST_F(ReportApiTest, GetLeaveReport_WithTypeFilter)
{
    auto data = ReportService::getInstance().getLeaveReport(
        "12-01", "12-31", "", "sick_leave");
    EXPECT_TRUE(data.isMember("leave_records"));
}

// ==================== 边界条件测试 ====================

TEST_F(ReportApiTest, GetDetailsReport_EmptyDateRange)
{
    auto data = ReportService::getInstance().getDetailsReport("", "", "", "");
    EXPECT_TRUE(data.isMember("records"));
}

TEST_F(ReportApiTest, GetDetailsReport_InvalidDateRange)
{
    auto data = ReportService::getInstance().getDetailsReport(
        "12-31", "12-01", "", "");  // End before start
    // Should handle gracefully
    EXPECT_TRUE(data.isMember("records"));
}

TEST_F(ReportApiTest, GetDetailsReport_SameDayRange)
{
    auto data = ReportService::getInstance().getDetailsReport(
        "12-15", "12-15", "", "");
    EXPECT_TRUE(data.isMember("records"));
}

TEST_F(ReportApiTest, GetDetailsReport_NonExistentClass)
{
    auto data = ReportService::getInstance().getDetailsReport(
        "12-01", "12-31", "不存在的班级", "");
    EXPECT_TRUE(data.isMember("records"));
}

TEST_F(ReportApiTest, GetDetailsReport_NonExistentStudent)
{
    auto data = ReportService::getInstance().getDetailsReport(
        "12-01", "12-31", "", "9999999");
    EXPECT_TRUE(data.isMember("records"));
}

TEST_F(ReportApiTest, GetDailyReport_EmptyDate)
{
    auto data = ReportService::getInstance().getDailyReport("", "");
    EXPECT_TRUE(data.isMember("date"));
}

TEST_F(ReportApiTest, GetDailyReport_NonExistentClass)
{
    auto data = ReportService::getInstance().getDailyReport("12-15", "不存在的班级");
    EXPECT_TRUE(data.isMember("details"));
}

TEST_F(ReportApiTest, GetSummaryReport_EmptyDateRange)
{
    auto data = ReportService::getInstance().getSummaryReport("", "", "");
    EXPECT_TRUE(data.isMember("summary"));
}

TEST_F(ReportApiTest, GetSummaryReport_NonExistentClass)
{
    auto data = ReportService::getInstance().getSummaryReport(
        "12-01", "12-31", "不存在的班级");
    EXPECT_TRUE(data.isMember("summary"));
}

TEST_F(ReportApiTest, GetAbnormalReport_EmptyDateRange)
{
    auto data = ReportService::getInstance().getAbnormalReport("", "", "", "");
    EXPECT_TRUE(data.isMember("abnormal_records"));
}

TEST_F(ReportApiTest, GetAbnormalReport_AllAbnormalTypes)
{
    std::vector<std::string> types = {"absent", "late", "early_leave"};
    for (const auto &type : types)
    {
        auto data = ReportService::getInstance().getAbnormalReport(
            "12-01", "12-31", "", type);
        EXPECT_TRUE(data.isMember("abnormal_records"))
            << "Failed for type: " << type;
    }
}

TEST_F(ReportApiTest, GetLeaveReport_EmptyDateRange)
{
    auto data = ReportService::getInstance().getLeaveReport("", "", "", "");
    EXPECT_TRUE(data.isMember("leave_records"));
}

TEST_F(ReportApiTest, GetLeaveReport_AllLeaveTypes)
{
    std::vector<std::string> types = {"personal_leave", "sick_leave"};
    for (const auto &type : types)
    {
        auto data = ReportService::getInstance().getLeaveReport(
            "12-01", "12-31", "", type);
        EXPECT_TRUE(data.isMember("leave_records"))
            << "Failed for type: " << type;
    }
}

TEST_F(ReportApiTest, GetDetailsReport_WithBothFilters)
{
    auto data = ReportService::getInstance().getDetailsReport(
        "12-01", "12-31", "人文2401班", "2024001");
    EXPECT_TRUE(data.isMember("records"));
}

TEST_F(ReportApiTest, GetAbnormalReport_WithBothFilters)
{
    auto data = ReportService::getInstance().getAbnormalReport(
        "12-01", "12-31", "人文2401班", "absent");
    EXPECT_TRUE(data.isMember("abnormal_records"));
}

TEST_F(ReportApiTest, GetLeaveReport_WithBothFilters)
{
    auto data = ReportService::getInstance().getLeaveReport(
        "12-01", "12-31", "人文2401班", "sick_leave");
    EXPECT_TRUE(data.isMember("leave_records"));
}

