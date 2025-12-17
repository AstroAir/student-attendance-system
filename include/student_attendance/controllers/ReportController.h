#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

namespace api
{
namespace v1
{

class ReportController : public drogon::HttpController<ReportController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ReportController::getDetailsReport, "/api/v1/reports/details", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(ReportController::getDailyReport, "/api/v1/reports/daily", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(ReportController::getSummaryReport, "/api/v1/reports/summary", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(ReportController::getAbnormalReport, "/api/v1/reports/abnormal", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(ReportController::getLeaveReport, "/api/v1/reports/leave", drogon::Get, "student_attendance::filters::AuthFilter");
    METHOD_LIST_END

    void getDetailsReport(const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void getDailyReport(const drogon::HttpRequestPtr &req,
                        std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void getSummaryReport(const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void getAbnormalReport(const drogon::HttpRequestPtr &req,
                           std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void getLeaveReport(const drogon::HttpRequestPtr &req,
                        std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;
};

}  // namespace v1
}  // namespace api

