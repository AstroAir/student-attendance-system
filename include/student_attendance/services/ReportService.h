#pragma once

#include <string>
#include <vector>
#include <json/json.h>
#include "student_attendance/models/Attendance.h"
#include "student_attendance/models/Student.h"
#include "student_attendance/models/DataStore.h"

namespace student_attendance
{
namespace services
{

class ReportService
{
public:
    static ReportService &getInstance()
    {
        static ReportService instance;
        return instance;
    }

    // 考勤明细表
    Json::Value getDetailsReport(const std::string &startDate,
                                 const std::string &endDate,
                                 const std::string &className,
                                 const std::string &studentId) const;

    // 考勤日报表
    Json::Value getDailyReport(const std::string &date,
                               const std::string &className) const;

    // 考勤汇总表
    Json::Value getSummaryReport(const std::string &startDate,
                                 const std::string &endDate,
                                 const std::string &className) const;

    // 考勤异常表
    Json::Value getAbnormalReport(const std::string &startDate,
                                  const std::string &endDate,
                                  const std::string &className,
                                  const std::string &type) const;

    // 请假汇总表
    Json::Value getLeaveReport(const std::string &startDate,
                               const std::string &endDate,
                               const std::string &className,
                               const std::string &type) const;

private:
    ReportService() = default;
    ~ReportService() = default;
    ReportService(const ReportService &) = delete;
    ReportService &operator=(const ReportService &) = delete;

    models::DataStore &dataStore_ = models::DataStore::getInstance();
};

}  // namespace services
}  // namespace student_attendance

