#include <gtest/gtest.h>
#include "student_attendance/utils/AttendanceStatus.h"

using namespace student_attendance::utils;

class AttendanceStatusTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ==================== getSymbol Tests ====================

TEST_F(AttendanceStatusTest, GetSymbol_Present)
{
    EXPECT_EQ(AttendanceStatus::getSymbol("present"), "√");
}

TEST_F(AttendanceStatusTest, GetSymbol_Absent)
{
    EXPECT_EQ(AttendanceStatus::getSymbol("absent"), "X");
}

TEST_F(AttendanceStatusTest, GetSymbol_PersonalLeave)
{
    EXPECT_EQ(AttendanceStatus::getSymbol("personal_leave"), "△");
}

TEST_F(AttendanceStatusTest, GetSymbol_SickLeave)
{
    EXPECT_EQ(AttendanceStatus::getSymbol("sick_leave"), "○");
}

TEST_F(AttendanceStatusTest, GetSymbol_Late)
{
    EXPECT_EQ(AttendanceStatus::getSymbol("late"), "+");
}

TEST_F(AttendanceStatusTest, GetSymbol_EarlyLeave)
{
    EXPECT_EQ(AttendanceStatus::getSymbol("early_leave"), "–");
}

TEST_F(AttendanceStatusTest, GetSymbol_InvalidStatus)
{
    EXPECT_EQ(AttendanceStatus::getSymbol("invalid"), "");
    EXPECT_EQ(AttendanceStatus::getSymbol(""), "");
    EXPECT_EQ(AttendanceStatus::getSymbol("PRESENT"), "");  // Case sensitive
}

// ==================== getStatusFromSymbol Tests ====================

TEST_F(AttendanceStatusTest, GetStatusFromSymbol_Present)
{
    EXPECT_EQ(AttendanceStatus::getStatusFromSymbol("√"), "present");
}

TEST_F(AttendanceStatusTest, GetStatusFromSymbol_Absent)
{
    EXPECT_EQ(AttendanceStatus::getStatusFromSymbol("X"), "absent");
}

TEST_F(AttendanceStatusTest, GetStatusFromSymbol_PersonalLeave)
{
    EXPECT_EQ(AttendanceStatus::getStatusFromSymbol("△"), "personal_leave");
}

TEST_F(AttendanceStatusTest, GetStatusFromSymbol_SickLeave)
{
    EXPECT_EQ(AttendanceStatus::getStatusFromSymbol("○"), "sick_leave");
}

TEST_F(AttendanceStatusTest, GetStatusFromSymbol_Late)
{
    EXPECT_EQ(AttendanceStatus::getStatusFromSymbol("+"), "late");
}

TEST_F(AttendanceStatusTest, GetStatusFromSymbol_EarlyLeave)
{
    EXPECT_EQ(AttendanceStatus::getStatusFromSymbol("–"), "early_leave");
}

TEST_F(AttendanceStatusTest, GetStatusFromSymbol_InvalidSymbol)
{
    EXPECT_EQ(AttendanceStatus::getStatusFromSymbol("?"), "");
    EXPECT_EQ(AttendanceStatus::getStatusFromSymbol(""), "");
    EXPECT_EQ(AttendanceStatus::getStatusFromSymbol("x"), "");  // lowercase
}

// ==================== isValidStatus Tests ====================

TEST_F(AttendanceStatusTest, IsValidStatus_AllValidStatuses)
{
    EXPECT_TRUE(AttendanceStatus::isValidStatus("present"));
    EXPECT_TRUE(AttendanceStatus::isValidStatus("absent"));
    EXPECT_TRUE(AttendanceStatus::isValidStatus("personal_leave"));
    EXPECT_TRUE(AttendanceStatus::isValidStatus("sick_leave"));
    EXPECT_TRUE(AttendanceStatus::isValidStatus("late"));
    EXPECT_TRUE(AttendanceStatus::isValidStatus("early_leave"));
}

TEST_F(AttendanceStatusTest, IsValidStatus_InvalidStatuses)
{
    EXPECT_FALSE(AttendanceStatus::isValidStatus("invalid"));
    EXPECT_FALSE(AttendanceStatus::isValidStatus(""));
    EXPECT_FALSE(AttendanceStatus::isValidStatus("PRESENT"));  // Case sensitive
    EXPECT_FALSE(AttendanceStatus::isValidStatus("Present"));
    EXPECT_FALSE(AttendanceStatus::isValidStatus("unknown"));
    EXPECT_FALSE(AttendanceStatus::isValidStatus(" present"));  // With space
}

// ==================== isAbnormalStatus Tests ====================

TEST_F(AttendanceStatusTest, IsAbnormalStatus_AbnormalStatuses)
{
    EXPECT_TRUE(AttendanceStatus::isAbnormalStatus("absent"));
    EXPECT_TRUE(AttendanceStatus::isAbnormalStatus("late"));
    EXPECT_TRUE(AttendanceStatus::isAbnormalStatus("early_leave"));
}

TEST_F(AttendanceStatusTest, IsAbnormalStatus_NormalStatuses)
{
    EXPECT_FALSE(AttendanceStatus::isAbnormalStatus("present"));
    EXPECT_FALSE(AttendanceStatus::isAbnormalStatus("personal_leave"));
    EXPECT_FALSE(AttendanceStatus::isAbnormalStatus("sick_leave"));
}

TEST_F(AttendanceStatusTest, IsAbnormalStatus_InvalidStatuses)
{
    EXPECT_FALSE(AttendanceStatus::isAbnormalStatus(""));
    EXPECT_FALSE(AttendanceStatus::isAbnormalStatus("invalid"));
}

// ==================== isLeaveStatus Tests ====================

TEST_F(AttendanceStatusTest, IsLeaveStatus_LeaveStatuses)
{
    EXPECT_TRUE(AttendanceStatus::isLeaveStatus("personal_leave"));
    EXPECT_TRUE(AttendanceStatus::isLeaveStatus("sick_leave"));
}

TEST_F(AttendanceStatusTest, IsLeaveStatus_NonLeaveStatuses)
{
    EXPECT_FALSE(AttendanceStatus::isLeaveStatus("present"));
    EXPECT_FALSE(AttendanceStatus::isLeaveStatus("absent"));
    EXPECT_FALSE(AttendanceStatus::isLeaveStatus("late"));
    EXPECT_FALSE(AttendanceStatus::isLeaveStatus("early_leave"));
}

TEST_F(AttendanceStatusTest, IsLeaveStatus_InvalidStatuses)
{
    EXPECT_FALSE(AttendanceStatus::isLeaveStatus(""));
    EXPECT_FALSE(AttendanceStatus::isLeaveStatus("invalid"));
}

// ==================== getChineseName Tests ====================

TEST_F(AttendanceStatusTest, GetChineseName_Present)
{
    EXPECT_EQ(AttendanceStatus::getChineseName("present"), "出勤");
}

TEST_F(AttendanceStatusTest, GetChineseName_Absent)
{
    EXPECT_EQ(AttendanceStatus::getChineseName("absent"), "旷课");
}

TEST_F(AttendanceStatusTest, GetChineseName_PersonalLeave)
{
    EXPECT_EQ(AttendanceStatus::getChineseName("personal_leave"), "事假");
}

TEST_F(AttendanceStatusTest, GetChineseName_SickLeave)
{
    EXPECT_EQ(AttendanceStatus::getChineseName("sick_leave"), "病假");
}

TEST_F(AttendanceStatusTest, GetChineseName_Late)
{
    EXPECT_EQ(AttendanceStatus::getChineseName("late"), "迟到");
}

TEST_F(AttendanceStatusTest, GetChineseName_EarlyLeave)
{
    EXPECT_EQ(AttendanceStatus::getChineseName("early_leave"), "早退");
}

TEST_F(AttendanceStatusTest, GetChineseName_InvalidStatus)
{
    EXPECT_EQ(AttendanceStatus::getChineseName("invalid"), "");
    EXPECT_EQ(AttendanceStatus::getChineseName(""), "");
}

// ==================== Constants Tests ====================

TEST_F(AttendanceStatusTest, Constants_Values)
{
    EXPECT_EQ(AttendanceStatus::PRESENT, "present");
    EXPECT_EQ(AttendanceStatus::ABSENT, "absent");
    EXPECT_EQ(AttendanceStatus::PERSONAL_LEAVE, "personal_leave");
    EXPECT_EQ(AttendanceStatus::SICK_LEAVE, "sick_leave");
    EXPECT_EQ(AttendanceStatus::LATE, "late");
    EXPECT_EQ(AttendanceStatus::EARLY_LEAVE, "early_leave");
}

TEST_F(AttendanceStatusTest, Constants_AreValidStatuses)
{
    EXPECT_TRUE(AttendanceStatus::isValidStatus(AttendanceStatus::PRESENT));
    EXPECT_TRUE(AttendanceStatus::isValidStatus(AttendanceStatus::ABSENT));
    EXPECT_TRUE(AttendanceStatus::isValidStatus(AttendanceStatus::PERSONAL_LEAVE));
    EXPECT_TRUE(AttendanceStatus::isValidStatus(AttendanceStatus::SICK_LEAVE));
    EXPECT_TRUE(AttendanceStatus::isValidStatus(AttendanceStatus::LATE));
    EXPECT_TRUE(AttendanceStatus::isValidStatus(AttendanceStatus::EARLY_LEAVE));
}

// ==================== Symbol-Status Consistency Tests ====================

TEST_F(AttendanceStatusTest, SymbolStatusRoundTrip)
{
    std::vector<std::string> statuses = {
        "present", "absent", "personal_leave", "sick_leave", "late", "early_leave"
    };

    for (const auto &status : statuses)
    {
        std::string symbol = AttendanceStatus::getSymbol(status);
        std::string recoveredStatus = AttendanceStatus::getStatusFromSymbol(symbol);
        EXPECT_EQ(status, recoveredStatus)
            << "Round trip failed for status: " << status;
    }
}

