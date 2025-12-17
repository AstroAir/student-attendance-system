#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

namespace api
{
namespace v1
{

class StudentController : public drogon::HttpController<StudentController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(StudentController::getStudents, "/api/v1/students", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(StudentController::createStudent, "/api/v1/students", drogon::Post, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(StudentController::getStudent, "/api/v1/students/{student_id}", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(StudentController::updateStudent, "/api/v1/students/{student_id}", drogon::Put, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(StudentController::deleteStudent, "/api/v1/students/{student_id}", drogon::Delete, "student_attendance::filters::AuthFilter");
    METHOD_LIST_END

    void getStudents(const drogon::HttpRequestPtr &req,
                     std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void createStudent(const drogon::HttpRequestPtr &req,
                       std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void getStudent(const drogon::HttpRequestPtr &req,
                    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                    const std::string &studentId) const;

    void updateStudent(const drogon::HttpRequestPtr &req,
                       std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                       const std::string &studentId) const;

    void deleteStudent(const drogon::HttpRequestPtr &req,
                       std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                       const std::string &studentId) const;
};

}  // namespace v1
}  // namespace api

