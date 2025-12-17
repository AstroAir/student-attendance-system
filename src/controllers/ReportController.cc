#include "student_attendance/controllers/ReportController.h"
#include "student_attendance/services/ReportService.h"
#include "student_attendance/utils/JsonResponse.h"

using namespace drogon;
using namespace student_attendance::services;
using namespace student_attendance::utils;

namespace api
{
namespace v1
{

void ReportController::getDetailsReport(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string startDate = req->getParameter("start_date");
    std::string endDate = req->getParameter("end_date");
    std::string className = req->getParameter("class");
    std::string studentId = req->getParameter("student_id");

    if (startDate.empty() || endDate.empty())
    {
        callback(JsonResponse::badRequest("start_date和end_date为必填参数"));
        return;
    }

    auto data = ReportService::getInstance().getDetailsReport(
        startDate, endDate, className, studentId);

    callback(JsonResponse::success(data));
}

void ReportController::getDailyReport(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string date = req->getParameter("date");
    std::string className = req->getParameter("class");

    if (date.empty())
    {
        callback(JsonResponse::badRequest("date为必填参数"));
        return;
    }

    auto data = ReportService::getInstance().getDailyReport(date, className);

    callback(JsonResponse::success(data));
}

void ReportController::getSummaryReport(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string startDate = req->getParameter("start_date");
    std::string endDate = req->getParameter("end_date");
    std::string className = req->getParameter("class");

    if (startDate.empty() || endDate.empty())
    {
        callback(JsonResponse::badRequest("start_date和end_date为必填参数"));
        return;
    }

    auto data = ReportService::getInstance().getSummaryReport(
        startDate, endDate, className);

    callback(JsonResponse::success(data));
}

void ReportController::getAbnormalReport(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string startDate = req->getParameter("start_date");
    std::string endDate = req->getParameter("end_date");
    std::string className = req->getParameter("class");
    std::string type = req->getParameter("type");

    if (startDate.empty() || endDate.empty())
    {
        callback(JsonResponse::badRequest("start_date和end_date为必填参数"));
        return;
    }

    auto data = ReportService::getInstance().getAbnormalReport(
        startDate, endDate, className, type);

    callback(JsonResponse::success(data));
}

void ReportController::getLeaveReport(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string startDate = req->getParameter("start_date");
    std::string endDate = req->getParameter("end_date");
    std::string className = req->getParameter("class");
    std::string type = req->getParameter("type");

    if (startDate.empty() || endDate.empty())
    {
        callback(JsonResponse::badRequest("start_date和end_date为必填参数"));
        return;
    }

    auto data = ReportService::getInstance().getLeaveReport(
        startDate, endDate, className, type);

    callback(JsonResponse::success(data));
}

}  // namespace v1
}  // namespace api


