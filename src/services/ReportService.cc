#include "student_attendance/services/ReportService.h"
#include "student_attendance/utils/AttendanceStatus.h"
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace student_attendance
{
namespace services
{

Json::Value ReportService::getDetailsReport(
    const std::string &startDate,
    const std::string &endDate,
    const std::string &className,
    const std::string &studentId) const
{
    Json::Value result;

    // Period
    Json::Value period;
    period["start_date"] = startDate;
    period["end_date"] = endDate;
    result["period"] = period;

    // Get students
    auto students = dataStore_.getAllStudents();
    if (!className.empty())
    {
        students.erase(
            std::remove_if(students.begin(), students.end(),
                [&className](const models::Student &s) {
                    return s.className != className;
                }),
            students.end());
    }
    if (!studentId.empty())
    {
        students.erase(
            std::remove_if(students.begin(), students.end(),
                [&studentId](const models::Student &s) {
                    return s.studentId != studentId;
                }),
            students.end());
    }

    // Get attendances
    auto attendances = dataStore_.searchAttendances(
        "", "", className, "", startDate, endDate, "");

    // Group attendances by student
    std::unordered_map<std::string, std::vector<models::Attendance>> studentAttendances;
    for (const auto &att : attendances)
    {
        studentAttendances[att.studentId].push_back(att);
    }

    Json::Value records(Json::arrayValue);
    for (const auto &student : students)
    {
        Json::Value record;
        record["student_id"] = student.studentId;
        record["name"] = student.name;
        record["class"] = student.className;

        Json::Value details(Json::arrayValue);
        auto it = studentAttendances.find(student.studentId);
        if (it != studentAttendances.end())
        {
            for (const auto &att : it->second)
            {
                Json::Value detail;
                detail["date"] = att.date;
                detail["status"] = att.status;
                detail["symbol"] = utils::AttendanceStatus::getSymbol(att.status);
                details.append(detail);
            }
        }
        record["attendance_details"] = details;
        records.append(record);
    }

    result["records"] = records;
    return result;
}

Json::Value ReportService::getDailyReport(
    const std::string &date,
    const std::string &className) const
{
    Json::Value result;
    result["date"] = date;

    // Get attendances for the date
    auto attendances = dataStore_.searchAttendances(
        "", "", className, date, "", "", "");

    // Count statistics
    int totalStudents = static_cast<int>(attendances.size());
    int present = 0, absent = 0, late = 0, earlyLeave = 0;
    int personalLeave = 0, sickLeave = 0;

    for (const auto &att : attendances)
    {
        if (att.status == "present") present++;
        else if (att.status == "absent") absent++;
        else if (att.status == "late") late++;
        else if (att.status == "early_leave") earlyLeave++;
        else if (att.status == "personal_leave") personalLeave++;
        else if (att.status == "sick_leave") sickLeave++;
    }

    // Calculate attendance rate
    double rate = totalStudents > 0 ?
        (static_cast<double>(present) / totalStudents * 100.0) : 0.0;
    std::ostringstream rateStr;
    rateStr << std::fixed << std::setprecision(2) << rate << "%";

    Json::Value summary;
    summary["total_students"] = totalStudents;
    summary["present"] = present;
    summary["absent"] = absent;
    summary["late"] = late;
    summary["early_leave"] = earlyLeave;
    summary["personal_leave"] = personalLeave;
    summary["sick_leave"] = sickLeave;
    summary["attendance_rate"] = rateStr.str();
    result["summary"] = summary;

    // Details
    Json::Value details(Json::arrayValue);
    for (const auto &att : attendances)
    {
        Json::Value detail;
        detail["student_id"] = att.studentId;
        detail["name"] = att.name;
        detail["class"] = att.className;
        detail["status"] = att.status;
        detail["symbol"] = utils::AttendanceStatus::getSymbol(att.status);
        details.append(detail);
    }
    result["details"] = details;

    return result;
}

Json::Value ReportService::getSummaryReport(
    const std::string &startDate,
    const std::string &endDate,
    const std::string &className) const
{
    Json::Value result;

    Json::Value period;
    period["start_date"] = startDate;
    period["end_date"] = endDate;
    result["period"] = period;

    // Get students
    auto students = dataStore_.getAllStudents();
    if (!className.empty())
    {
        students.erase(
            std::remove_if(students.begin(), students.end(),
                [&className](const models::Student &s) {
                    return s.className != className;
                }),
            students.end());
    }

    // Get attendances
    auto attendances = dataStore_.searchAttendances(
        "", "", className, "", startDate, endDate, "");

    // Group by student
    std::unordered_map<std::string, std::vector<models::Attendance>> studentAttendances;
    for (const auto &att : attendances)
    {
        studentAttendances[att.studentId].push_back(att);
    }

    Json::Value summaryArray(Json::arrayValue);
    for (const auto &student : students)
    {
        Json::Value item;
        item["student_id"] = student.studentId;
        item["name"] = student.name;
        item["class"] = student.className;

        int totalDays = 0, presentCount = 0, absentCount = 0;
        int lateCount = 0, earlyLeaveCount = 0;
        int personalLeaveCount = 0, sickLeaveCount = 0;

        auto it = studentAttendances.find(student.studentId);
        if (it != studentAttendances.end())
        {
            totalDays = static_cast<int>(it->second.size());
            for (const auto &att : it->second)
            {
                if (att.status == "present") presentCount++;
                else if (att.status == "absent") absentCount++;
                else if (att.status == "late") lateCount++;
                else if (att.status == "early_leave") earlyLeaveCount++;
                else if (att.status == "personal_leave") personalLeaveCount++;
                else if (att.status == "sick_leave") sickLeaveCount++;
            }
        }

        double rate = totalDays > 0 ?
            (static_cast<double>(presentCount) / totalDays * 100.0) : 0.0;
        std::ostringstream rateStr;
        rateStr << std::fixed << std::setprecision(2) << rate << "%";

        item["total_days"] = totalDays;
        item["present_count"] = presentCount;
        item["absent_count"] = absentCount;
        item["late_count"] = lateCount;
        item["early_leave_count"] = earlyLeaveCount;
        item["personal_leave_count"] = personalLeaveCount;
        item["sick_leave_count"] = sickLeaveCount;
        item["attendance_rate"] = rateStr.str();

        summaryArray.append(item);
    }

    result["summary"] = summaryArray;
    return result;
}

Json::Value ReportService::getAbnormalReport(
    const std::string &startDate,
    const std::string &endDate,
    const std::string &className,
    const std::string &type) const
{
    Json::Value result;

    Json::Value period;
    period["start_date"] = startDate;
    period["end_date"] = endDate;
    result["period"] = period;

    // Get attendances
    auto attendances = dataStore_.searchAttendances(
        "", "", className, "", startDate, endDate, "");

    // Filter abnormal records
    std::vector<models::Attendance> abnormalRecords;
    int absentCount = 0, lateCount = 0, earlyLeaveCount = 0;

    for (const auto &att : attendances)
    {
        bool isAbnormal = false;
        if (type.empty())
        {
            isAbnormal = utils::AttendanceStatus::isAbnormalStatus(att.status);
        }
        else
        {
            isAbnormal = (att.status == type);
        }

        if (isAbnormal)
        {
            abnormalRecords.push_back(att);
            if (att.status == "absent") absentCount++;
            else if (att.status == "late") lateCount++;
            else if (att.status == "early_leave") earlyLeaveCount++;
        }
    }

    Json::Value records(Json::arrayValue);
    for (const auto &att : abnormalRecords)
    {
        Json::Value record;
        record["student_id"] = att.studentId;
        record["name"] = att.name;
        record["class"] = att.className;
        record["date"] = att.date;
        record["status"] = att.status;
        record["symbol"] = utils::AttendanceStatus::getSymbol(att.status);
        record["remark"] = att.remark;
        records.append(record);
    }
    result["abnormal_records"] = records;

    Json::Value statistics;
    statistics["total_abnormal"] = static_cast<int>(abnormalRecords.size());
    statistics["absent_count"] = absentCount;
    statistics["late_count"] = lateCount;
    statistics["early_leave_count"] = earlyLeaveCount;
    result["statistics"] = statistics;

    return result;
}

Json::Value ReportService::getLeaveReport(
    const std::string &startDate,
    const std::string &endDate,
    const std::string &className,
    const std::string &type) const
{
    Json::Value result;

    Json::Value period;
    period["start_date"] = startDate;
    period["end_date"] = endDate;
    result["period"] = period;

    // Get attendances
    auto attendances = dataStore_.searchAttendances(
        "", "", className, "", startDate, endDate, "");

    // Filter leave records
    std::vector<models::Attendance> leaveRecords;
    int personalLeaveCount = 0, sickLeaveCount = 0;

    for (const auto &att : attendances)
    {
        bool isLeave = false;
        if (type.empty())
        {
            isLeave = utils::AttendanceStatus::isLeaveStatus(att.status);
        }
        else
        {
            isLeave = (att.status == type);
        }

        if (isLeave)
        {
            leaveRecords.push_back(att);
            if (att.status == "personal_leave") personalLeaveCount++;
            else if (att.status == "sick_leave") sickLeaveCount++;
        }
    }

    Json::Value records(Json::arrayValue);
    for (const auto &att : leaveRecords)
    {
        Json::Value record;
        record["student_id"] = att.studentId;
        record["name"] = att.name;
        record["class"] = att.className;
        record["date"] = att.date;
        record["type"] = att.status;
        record["symbol"] = utils::AttendanceStatus::getSymbol(att.status);
        record["remark"] = att.remark;
        records.append(record);
    }
    result["leave_records"] = records;

    Json::Value statistics;
    statistics["total_leave"] = static_cast<int>(leaveRecords.size());
    statistics["personal_leave_count"] = personalLeaveCount;
    statistics["sick_leave_count"] = sickLeaveCount;
    result["statistics"] = statistics;

    return result;
}

}  // namespace services
}  // namespace student_attendance

