#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace student_attendance
{
namespace utils
{

class AttendanceStatus
{
public:
    static const std::string PRESENT;
    static const std::string ABSENT;
    static const std::string PERSONAL_LEAVE;
    static const std::string SICK_LEAVE;
    static const std::string LATE;
    static const std::string EARLY_LEAVE;

    static std::string getSymbol(const std::string &status)
    {
        static const std::unordered_map<std::string, std::string> symbolMap = {
            {"present", "√"},
            {"absent", "X"},
            {"personal_leave", "△"},
            {"sick_leave", "○"},
            {"late", "+"},
            {"early_leave", "–"}
        };

        auto it = symbolMap.find(status);
        if (it != symbolMap.end())
        {
            return it->second;
        }
        return "";
    }

    static std::string getStatusFromSymbol(const std::string &symbol)
    {
        static const std::unordered_map<std::string, std::string> statusMap = {
            {"√", "present"},
            {"X", "absent"},
            {"△", "personal_leave"},
            {"○", "sick_leave"},
            {"+", "late"},
            {"–", "early_leave"}
        };

        auto it = statusMap.find(symbol);
        if (it != statusMap.end())
        {
            return it->second;
        }
        return "";
    }

    static bool isValidStatus(const std::string &status)
    {
        static const std::vector<std::string> validStatuses = {
            "present", "absent", "personal_leave", "sick_leave", "late", "early_leave"
        };

        for (const auto &s : validStatuses)
        {
            if (s == status)
                return true;
        }
        return false;
    }

    static bool isAbnormalStatus(const std::string &status)
    {
        return status == "absent" || status == "late" || status == "early_leave";
    }

    static bool isLeaveStatus(const std::string &status)
    {
        return status == "personal_leave" || status == "sick_leave";
    }

    static std::string getChineseName(const std::string &status)
    {
        static const std::unordered_map<std::string, std::string> nameMap = {
            {"present", "出勤"},
            {"absent", "旷课"},
            {"personal_leave", "事假"},
            {"sick_leave", "病假"},
            {"late", "迟到"},
            {"early_leave", "早退"}
        };

        auto it = nameMap.find(status);
        if (it != nameMap.end())
        {
            return it->second;
        }
        return "";
    }
};

inline const std::string AttendanceStatus::PRESENT = "present";
inline const std::string AttendanceStatus::ABSENT = "absent";
inline const std::string AttendanceStatus::PERSONAL_LEAVE = "personal_leave";
inline const std::string AttendanceStatus::SICK_LEAVE = "sick_leave";
inline const std::string AttendanceStatus::LATE = "late";
inline const std::string AttendanceStatus::EARLY_LEAVE = "early_leave";

}  // namespace utils
}  // namespace student_attendance

