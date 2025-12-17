#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

namespace api
{
namespace v1
{

class AttendanceController : public drogon::HttpController<AttendanceController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AttendanceController::getAttendances, "/api/v1/attendances", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(AttendanceController::createAttendance, "/api/v1/attendances", drogon::Post, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(AttendanceController::batchCreateAttendances, "/api/v1/attendances/batch", drogon::Post, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(AttendanceController::getAttendance, "/api/v1/attendances/{id}", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(AttendanceController::updateAttendance, "/api/v1/attendances/{id}", drogon::Put, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(AttendanceController::deleteAttendance, "/api/v1/attendances/{id}", drogon::Delete, "student_attendance::filters::AuthFilter");
    METHOD_LIST_END

    void getAttendances(const drogon::HttpRequestPtr &req,
                        std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void createAttendance(const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void batchCreateAttendances(const drogon::HttpRequestPtr &req,
                                std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void getAttendance(const drogon::HttpRequestPtr &req,
                       std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                       int id) const;

    void updateAttendance(const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                          int id) const;

    void deleteAttendance(const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                          int id) const;
};

}  // namespace v1
}  // namespace api

