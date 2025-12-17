#pragma once

#include <string>
#include <vector>
#include <optional>
#include <json/json.h>
#include "student_attendance/models/Attendance.h"
#include "student_attendance/models/DataStore.h"

namespace student_attendance
{
namespace services
{

class AttendanceService
{
public:
    static AttendanceService &getInstance()
    {
        static AttendanceService instance;
        return instance;
    }

    struct AttendanceListResult
    {
        std::vector<models::Attendance> attendances;
        int total;
        int page;
        int pageSize;
    };

    AttendanceListResult getAttendances(
        int page, int pageSize,
        const std::string &studentId,
        const std::string &name,
        const std::string &className,
        const std::string &date,
        const std::string &startDate,
        const std::string &endDate,
        const std::string &status,
        const std::string &sortBy,
        const std::string &order) const;

    std::optional<models::Attendance> getAttendance(int id) const;

    std::pair<bool, models::Attendance> createAttendance(
        const std::string &studentId,
        const std::string &date,
        const std::string &status,
        const std::string &remark);

    int batchCreateAttendances(
        const std::string &date,
        const std::vector<std::pair<std::string, std::string>> &records);

    std::pair<bool, std::string> updateAttendance(
        int id,
        const std::string &status,
        const std::string &remark);

    bool deleteAttendance(int id);

private:
    AttendanceService() = default;
    ~AttendanceService() = default;
    AttendanceService(const AttendanceService &) = delete;
    AttendanceService &operator=(const AttendanceService &) = delete;

    models::DataStore &dataStore_ = models::DataStore::getInstance();
};

}  // namespace services
}  // namespace student_attendance

