#include "student_attendance/services/AttendanceService.h"
#include "student_attendance/utils/AttendanceStatus.h"
#include "student_attendance/db/DatabaseManager.h"
#include <algorithm>
#include <drogon/orm/DbClient.h>

namespace student_attendance
{
namespace services
{

AttendanceService::AttendanceListResult AttendanceService::getAttendances(
    int page, int pageSize,
    const std::string &studentId,
    const std::string &name,
    const std::string &className,
    const std::string &date,
    const std::string &startDate,
    const std::string &endDate,
    const std::string &status,
    const std::string &sortBy,
    const std::string &order) const
{
    auto fallback = [&]() -> AttendanceListResult {
        auto attendances = dataStore_.searchAttendances(
            studentId, name, className, date, startDate, endDate, status);

        // Sort
        if (!sortBy.empty())
        {
            bool ascending = (order != "desc");
            std::sort(attendances.begin(), attendances.end(),
                [&sortBy, ascending](const models::Attendance &a, const models::Attendance &b) {
                    int cmp = 0;
                    if (sortBy == "student_id")
                    {
                        cmp = a.studentId.compare(b.studentId);
                    }
                    else if (sortBy == "name")
                    {
                        cmp = a.name.compare(b.name);
                    }
                    else if (sortBy == "date")
                    {
                        cmp = a.date.compare(b.date);
                    }
                    return ascending ? (cmp < 0) : (cmp > 0);
                });
        }

        int total = static_cast<int>(attendances.size());

        // Pagination
        int startIndex = (page - 1) * pageSize;
        int endIndex = std::min(startIndex + pageSize, total);

        std::vector<models::Attendance> pagedAttendances;
        if (startIndex < total)
        {
            pagedAttendances.assign(attendances.begin() + startIndex,
                                   attendances.begin() + endIndex);
        }

        return {pagedAttendances, total, page, pageSize};
    };

    auto client = db::DatabaseManager::getInstance().getClient();
    if (!client)
    {
        return fallback();
    }

    try
    {
        std::string sortCol;
        if (sortBy == "student_id")
            sortCol = "a.student_id";
        else if (sortBy == "name")
            sortCol = "s.name";
        else if (sortBy == "date")
            sortCol = "a.date";
        if (sortCol.empty())
            sortCol = "a.id";

        std::string sortOrder = (order == "desc") ? "DESC" : "ASC";

        std::string whereSql = " WHERE 1=1";
        std::vector<std::string> args;

        if (!studentId.empty())
        {
            whereSql += " AND a.student_id = ?";
            args.push_back(studentId);
        }
        if (!name.empty())
        {
            whereSql += " AND s.name LIKE ?";
            args.push_back("%" + name + "%");
        }
        if (!className.empty())
        {
            whereSql += " AND s.class_name = ?";
            args.push_back(className);
        }
        if (!date.empty())
        {
            whereSql += " AND a.date = ?";
            args.push_back(date);
        }
        if (!startDate.empty())
        {
            whereSql += " AND a.date >= ?";
            args.push_back(startDate);
        }
        if (!endDate.empty())
        {
            whereSql += " AND a.date <= ?";
            args.push_back(endDate);
        }
        if (!status.empty())
        {
            whereSql += " AND a.status = ?";
            args.push_back(status);
        }

        int total = 0;
        {
            std::string countSql =
                "SELECT COUNT(1) AS cnt "
                "FROM attendances a "
                "JOIN students s ON a.student_id = s.student_id" + whereSql;

            auto binder = (*client) << countSql;
            for (const auto &arg : args)
            {
                binder << arg;
            }

            drogon::orm::Result r(nullptr);
            binder << drogon::orm::Mode::Blocking;
            binder >> [&r](const drogon::orm::Result &result) { r = result; };
            binder.exec();

            if (!r.empty())
            {
                total = r[0]["cnt"].as<int>();
            }
        }

        int offset = (page - 1) * pageSize;
        std::string querySql =
            "SELECT "
            "  a.id AS id, "
            "  a.student_id AS student_id, "
            "  s.name AS name, "
            "  s.class_name AS class_name, "
            "  a.date AS date, "
            "  a.status AS status, "
            "  a.remark AS remark "
            "FROM attendances a "
            "JOIN students s ON a.student_id = s.student_id" + whereSql +
            " ORDER BY " + sortCol + " " + sortOrder +
            " LIMIT ? OFFSET ?";

        std::vector<models::Attendance> pagedAttendances;
        {
            auto binder = (*client) << querySql;
            for (const auto &arg : args)
            {
                binder << arg;
            }
            binder << pageSize << offset;

            drogon::orm::Result r(nullptr);
            binder << drogon::orm::Mode::Blocking;
            binder >> [&r](const drogon::orm::Result &result) { r = result; };
            binder.exec();

            pagedAttendances.reserve(r.size());
            for (const auto &row : r)
            {
                models::Attendance a;
                a.id = row["id"].as<int>();
                a.studentId = row["student_id"].as<std::string>();
                a.name = row["name"].as<std::string>();
                a.className = row["class_name"].as<std::string>();
                a.date = row["date"].as<std::string>();
                a.status = row["status"].as<std::string>();
                a.remark = row["remark"].isNull() ? "" : row["remark"].as<std::string>();
                pagedAttendances.push_back(std::move(a));
            }
        }

        return {pagedAttendances, total, page, pageSize};
    }
    catch (const drogon::orm::DrogonDbException &)
    {
    }
    catch (const std::exception &)
    {
    }

    return fallback();
}

std::optional<models::Attendance> AttendanceService::getAttendance(int id) const
{
    auto client = db::DatabaseManager::getInstance().getClient();
    if (!client)
    {
        return dataStore_.getAttendanceById(id);
    }

    try
    {
        auto r = client->execSqlSync(
            "SELECT "
            "  a.id AS id, "
            "  a.student_id AS student_id, "
            "  s.name AS name, "
            "  s.class_name AS class_name, "
            "  a.date AS date, "
            "  a.status AS status, "
            "  a.remark AS remark "
            "FROM attendances a "
            "JOIN students s ON a.student_id = s.student_id "
            "WHERE a.id = ?",
            id);

        if (r.empty())
        {
            return std::nullopt;
        }

        models::Attendance a;
        a.id = r[0]["id"].as<int>();
        a.studentId = r[0]["student_id"].as<std::string>();
        a.name = r[0]["name"].as<std::string>();
        a.className = r[0]["class_name"].as<std::string>();
        a.date = r[0]["date"].as<std::string>();
        a.status = r[0]["status"].as<std::string>();
        a.remark = r[0]["remark"].isNull() ? "" : r[0]["remark"].as<std::string>();
        return a;
    }
    catch (const drogon::orm::DrogonDbException &)
    {
    }
    catch (const std::exception &)
    {
    }

    return dataStore_.getAttendanceById(id);
}

std::pair<bool, models::Attendance> AttendanceService::createAttendance(
    const std::string &studentId,
    const std::string &date,
    const std::string &status,
    const std::string &remark)
{
    auto student = dataStore_.getStudentById(studentId);
    if (!student)
    {
        return {false, models::Attendance()};
    }

    if (!utils::AttendanceStatus::isValidStatus(status))
    {
        return {false, models::Attendance()};
    }

    models::Attendance att;
    att.studentId = studentId;
    att.name = student->name;
    att.className = student->className;
    att.date = date;
    att.status = status;
    att.remark = remark;

    int id = dataStore_.addAttendance(att);
    att.id = id;

    return {true, att};
}

int AttendanceService::batchCreateAttendances(
    const std::string &date,
    const std::vector<std::pair<std::string, std::string>> &records)
{
    int count = 0;
    for (const auto &[studentId, status] : records)
    {
        auto result = createAttendance(studentId, date, status, "");
        if (result.first)
        {
            count++;
        }
    }
    return count;
}

std::pair<bool, std::string> AttendanceService::updateAttendance(
    int id,
    const std::string &status,
    const std::string &remark)
{
    auto att = dataStore_.getAttendanceById(id);
    if (!att)
    {
        return {false, "考勤记录不存在"};
    }

    if (!status.empty() && !utils::AttendanceStatus::isValidStatus(status))
    {
        return {false, "无效的考勤状态"};
    }

    models::Attendance updateData;
    updateData.status = status;
    updateData.remark = remark;

    if (dataStore_.updateAttendance(id, updateData))
    {
        return {true, "考勤记录更新成功"};
    }
    return {false, "更新失败"};
}

bool AttendanceService::deleteAttendance(int id)
{
    return dataStore_.deleteAttendance(id);
}

}  // namespace services
}  // namespace student_attendance

