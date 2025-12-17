#pragma once

#include <string>
#include <vector>
#include <optional>
#include <json/json.h>
#include "student_attendance/models/Student.h"
#include "student_attendance/models/DataStore.h"

namespace student_attendance
{
namespace services
{

class StudentService
{
public:
    static StudentService &getInstance()
    {
        static StudentService instance;
        return instance;
    }

    struct StudentListResult
    {
        std::vector<models::Student> students;
        int total;
        int page;
        int pageSize;
    };

    StudentListResult getStudents(int page, int pageSize,
                                  const std::string &sortBy,
                                  const std::string &order,
                                  const std::string &className,
                                  const std::string &keyword) const;

    std::optional<models::Student> getStudent(const std::string &studentId) const;

    std::pair<bool, std::string> createStudent(const models::Student &student);

    std::pair<bool, std::string> updateStudent(const std::string &studentId,
                                               const std::string &name,
                                               const std::string &className);

    bool deleteStudent(const std::string &studentId);

private:
    StudentService() = default;
    ~StudentService() = default;
    StudentService(const StudentService &) = delete;
    StudentService &operator=(const StudentService &) = delete;

    models::DataStore &dataStore_ = models::DataStore::getInstance();
};

}  // namespace services
}  // namespace student_attendance

