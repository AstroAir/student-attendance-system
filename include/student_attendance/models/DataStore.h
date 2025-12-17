#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <optional>
#include <algorithm>
#include <memory>
#include "Student.h"
#include "Attendance.h"

namespace drogon
{
namespace orm
{
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
}  // namespace orm
}  // namespace drogon

namespace student_attendance
{
namespace models
{

class DataStore
{
public:
    static DataStore &getInstance()
    {
        static DataStore instance;
        return instance;
    }

    // Student operations
    std::vector<Student> getAllStudents() const;
    std::optional<Student> getStudentById(const std::string &studentId) const;
    bool addStudent(const Student &student);
    bool updateStudent(const std::string &studentId, const Student &student);
    bool deleteStudent(const std::string &studentId);
    bool studentExists(const std::string &studentId) const;
    std::vector<Student> searchStudents(const std::string &keyword,
                                        const std::string &className) const;

    // Attendance operations
    std::vector<Attendance> getAllAttendances() const;
    std::optional<Attendance> getAttendanceById(int id) const;
    int addAttendance(const Attendance &attendance);
    bool updateAttendance(int id, const Attendance &attendance);
    bool deleteAttendance(int id);
    std::vector<Attendance> searchAttendances(
        const std::string &studentId,
        const std::string &name,
        const std::string &className,
        const std::string &date,
        const std::string &startDate,
        const std::string &endDate,
        const std::string &status) const;

    // Class operations
    std::vector<std::string> getAllClasses() const;
    std::vector<Student> getStudentsByClass(const std::string &className) const;
    int getClassStudentCount(const std::string &className) const;

    // Data import/export
    void clear();
    void importStudents(const std::vector<Student> &students);
    void importAttendances(const std::vector<Attendance> &attendances);

    // For tests
    void reset();

private:
    DataStore();
    ~DataStore() = default;
    DataStore(const DataStore &) = delete;
    DataStore &operator=(const DataStore &) = delete;

    void initSampleData();
    void ensureDbClient();

    mutable std::mutex studentMutex_;
    mutable std::mutex attendanceMutex_;

    std::unordered_map<std::string, Student> students_;
    std::unordered_map<int, Attendance> attendances_;
    int nextAttendanceId_;

    drogon::orm::DbClientPtr dbClient_;
};

}  // namespace models
}  // namespace student_attendance

