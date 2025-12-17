#include "student_attendance/services/StudentService.h"
#include "student_attendance/db/DatabaseManager.h"
#include <algorithm>
#include <drogon/orm/DbClient.h>

namespace student_attendance
{
namespace services
{

StudentService::StudentListResult StudentService::getStudents(
    int page, int pageSize,
    const std::string &sortBy,
    const std::string &order,
    const std::string &className,
    const std::string &keyword) const
{
    auto client = db::DatabaseManager::getInstance().getClient();
    if (!client)
    {
        auto students = dataStore_.searchStudents(keyword, className);

        // Sort
        if (!sortBy.empty())
        {
            bool ascending = (order != "desc");
            std::sort(students.begin(), students.end(),
                [&sortBy, ascending](const models::Student &a, const models::Student &b) {
                    int cmp = 0;
                    if (sortBy == "student_id")
                    {
                        cmp = a.studentId.compare(b.studentId);
                    }
                    else if (sortBy == "name")
                    {
                        cmp = a.name.compare(b.name);
                    }
                    else if (sortBy == "class")
                    {
                        cmp = a.className.compare(b.className);
                    }
                    return ascending ? (cmp < 0) : (cmp > 0);
                });
        }

        int total = static_cast<int>(students.size());

        // Pagination
        int startIndex = (page - 1) * pageSize;
        int endIndex = std::min(startIndex + pageSize, total);

        std::vector<models::Student> pagedStudents;
        if (startIndex < total)
        {
            pagedStudents.assign(students.begin() + startIndex,
                                students.begin() + endIndex);
        }

        return {pagedStudents, total, page, pageSize};
    }

    try
    {
        std::string sortCol;
        if (sortBy == "student_id")
            sortCol = "student_id";
        else if (sortBy == "name")
            sortCol = "name";
        else if (sortBy == "class")
            sortCol = "class_name";

        std::string sortOrder = (order == "desc") ? "DESC" : "ASC";
        if (sortCol.empty())
        {
            sortCol = "student_id";
        }

        std::string whereSql = " WHERE 1=1";
        std::vector<std::string> stringArgs;

        if (!className.empty())
        {
            whereSql += " AND class_name = ?";
            stringArgs.push_back(className);
        }
        if (!keyword.empty())
        {
            whereSql += " AND (student_id LIKE ? OR name LIKE ?)";
            std::string pattern = "%" + keyword + "%";
            stringArgs.push_back(pattern);
            stringArgs.push_back(pattern);
        }

        int total = 0;
        {
            std::string countSql = "SELECT COUNT(1) AS cnt FROM students" + whereSql;
            auto binder = (*client) << countSql;
            for (const auto &arg : stringArgs)
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
            "SELECT student_id, name, class_name FROM students" + whereSql +
            " ORDER BY " + sortCol + " " + sortOrder +
            " LIMIT ? OFFSET ?";

        std::vector<models::Student> students;
        {
            auto binder = (*client) << querySql;
            for (const auto &arg : stringArgs)
            {
                binder << arg;
            }
            binder << pageSize << offset;

            drogon::orm::Result r(nullptr);
            binder << drogon::orm::Mode::Blocking;
            binder >> [&r](const drogon::orm::Result &result) { r = result; };
            binder.exec();

            students.reserve(r.size());
            for (const auto &row : r)
            {
                models::Student s;
                s.studentId = row["student_id"].as<std::string>();
                s.name = row["name"].as<std::string>();
                s.className = row["class_name"].as<std::string>();
                students.push_back(std::move(s));
            }
        }

        return {students, total, page, pageSize};
    }
    catch (const drogon::orm::DrogonDbException &)
    {
    }
    catch (const std::exception &)
    {
    }

    auto students = dataStore_.searchStudents(keyword, className);

    if (!sortBy.empty())
    {
        bool ascending = (order != "desc");
        std::sort(students.begin(), students.end(),
            [&sortBy, ascending](const models::Student &a, const models::Student &b) {
                int cmp = 0;
                if (sortBy == "student_id")
                {
                    cmp = a.studentId.compare(b.studentId);
                }
                else if (sortBy == "name")
                {
                    cmp = a.name.compare(b.name);
                }
                else if (sortBy == "class")
                {
                    cmp = a.className.compare(b.className);
                }
                return ascending ? (cmp < 0) : (cmp > 0);
            });
    }

    int total = static_cast<int>(students.size());
    int startIndex = (page - 1) * pageSize;
    int endIndex = std::min(startIndex + pageSize, total);

    std::vector<models::Student> pagedStudents;
    if (startIndex < total)
    {
        pagedStudents.assign(students.begin() + startIndex,
                            students.begin() + endIndex);
    }
    return {pagedStudents, total, page, pageSize};

}

std::optional<models::Student> StudentService::getStudent(
    const std::string &studentId) const
{
    auto client = db::DatabaseManager::getInstance().getClient();
    if (!client)
    {
        return dataStore_.getStudentById(studentId);
    }

    try
    {
        auto r = client->execSqlSync(
            "SELECT student_id, name, class_name FROM students WHERE student_id = ?",
            studentId);
        if (r.empty())
        {
            return std::nullopt;
        }

        models::Student s;
        s.studentId = r[0]["student_id"].as<std::string>();
        s.name = r[0]["name"].as<std::string>();
        s.className = r[0]["class_name"].as<std::string>();
        return s;
    }
    catch (const drogon::orm::DrogonDbException &)
    {
    }
    catch (const std::exception &)
    {
    }

    return dataStore_.getStudentById(studentId);
}

std::pair<bool, std::string> StudentService::createStudent(
    const models::Student &student)
{
    if (student.studentId.empty())
    {
        return {false, "学号不能为空"};
    }
    if (student.name.empty())
    {
        return {false, "姓名不能为空"};
    }
    if (student.className.empty())
    {
        return {false, "班级不能为空"};
    }

    auto client = db::DatabaseManager::getInstance().getClient();
    if (!client)
    {
        if (dataStore_.studentExists(student.studentId))
        {
            return {false, "学号已存在，不可重复添加"};
        }

        if (dataStore_.addStudent(student))
        {
            return {true, "学生创建成功"};
        }
        return {false, "创建失败"};
    }

    try
    {
        auto exists = client->execSqlSync(
            "SELECT 1 FROM students WHERE student_id = ? LIMIT 1",
            student.studentId);
        if (!exists.empty())
        {
            return {false, "学号已存在，不可重复添加"};
        }

        client->execSqlSync(
            "INSERT INTO students (student_id, name, class_name) VALUES (?, ?, ?)",
            student.studentId,
            student.name,
            student.className);
        return {true, "学生创建成功"};
    }
    catch (const drogon::orm::DrogonDbException &)
    {
    }
    catch (const std::exception &)
    {
    }

    if (dataStore_.studentExists(student.studentId))
    {
        return {false, "学号已存在，不可重复添加"};
    }

    if (dataStore_.addStudent(student))
    {
        return {true, "学生创建成功"};
    }
    return {false, "创建失败"};
}

std::pair<bool, std::string> StudentService::updateStudent(
    const std::string &studentId,
    const std::string &name,
    const std::string &className)
{
    auto client = db::DatabaseManager::getInstance().getClient();
    if (!client)
    {
        if (!dataStore_.studentExists(studentId))
        {
            return {false, "学生不存在"};
        }

        models::Student updateData;
        updateData.name = name;
        updateData.className = className;

        if (dataStore_.updateStudent(studentId, updateData))
        {
            return {true, "学生信息更新成功"};
        }
        return {false, "更新失败"};
    }

    try
    {
        auto exists = client->execSqlSync(
            "SELECT 1 FROM students WHERE student_id = ? LIMIT 1",
            studentId);
        if (exists.empty())
        {
            return {false, "学生不存在"};
        }

        auto r = client->execSqlSync(
            "UPDATE students "
            "SET name = COALESCE(NULLIF(?, ''), name), "
            "    class_name = COALESCE(NULLIF(?, ''), class_name) "
            "WHERE student_id = ?",
            name,
            className,
            studentId);

        if (r.affectedRows() > 0)
        {
            return {true, "学生信息更新成功"};
        }
        return {false, "更新失败"};
    }
    catch (const drogon::orm::DrogonDbException &)
    {
    }
    catch (const std::exception &)
    {
    }

    if (!dataStore_.studentExists(studentId))
    {
        return {false, "学生不存在"};
    }

    models::Student updateData;
    updateData.name = name;
    updateData.className = className;

    if (dataStore_.updateStudent(studentId, updateData))
    {
        return {true, "学生信息更新成功"};
    }
    return {false, "更新失败"};
}

bool StudentService::deleteStudent(const std::string &studentId)
{
    auto client = db::DatabaseManager::getInstance().getClient();
    if (!client)
    {
        return dataStore_.deleteStudent(studentId);
    }

    try
    {
        auto r = client->execSqlSync(
            "DELETE FROM students WHERE student_id = ?",
            studentId);
        return r.affectedRows() > 0;
    }
    catch (const drogon::orm::DrogonDbException &)
    {
    }
    catch (const std::exception &)
    {
    }

    return dataStore_.deleteStudent(studentId);
}

}  // namespace services
}  // namespace student_attendance

