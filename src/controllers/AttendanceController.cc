#include "student_attendance/controllers/AttendanceController.h"
#include "student_attendance/services/AttendanceService.h"
#include "student_attendance/utils/JsonResponse.h"

using namespace drogon;
using namespace student_attendance::services;
using namespace student_attendance::utils;

namespace api
{
namespace v1
{

void AttendanceController::getAttendances(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    int page = 1;
    int pageSize = 20;
    std::string studentId;
    std::string name;
    std::string className;
    std::string date;
    std::string startDate;
    std::string endDate;
    std::string status;
    std::string sortBy;
    std::string order = "asc";

    auto pageParam = req->getParameter("page");
    if (!pageParam.empty())
    {
        page = std::stoi(pageParam);
        if (page < 1) page = 1;
    }

    auto pageSizeParam = req->getParameter("page_size");
    if (!pageSizeParam.empty())
    {
        pageSize = std::stoi(pageSizeParam);
        if (pageSize < 1) pageSize = 1;
        if (pageSize > 100) pageSize = 100;
    }

    studentId = req->getParameter("student_id");
    name = req->getParameter("name");
    className = req->getParameter("class");
    date = req->getParameter("date");
    startDate = req->getParameter("start_date");
    endDate = req->getParameter("end_date");
    status = req->getParameter("status");
    sortBy = req->getParameter("sort_by");

    auto orderParam = req->getParameter("order");
    if (!orderParam.empty())
    {
        order = orderParam;
    }

    auto result = AttendanceService::getInstance().getAttendances(
        page, pageSize, studentId, name, className, date,
        startDate, endDate, status, sortBy, order);

    Json::Value items(Json::arrayValue);
    for (const auto &att : result.attendances)
    {
        items.append(att.toJson());
    }

    auto data = JsonResponse::paginatedData(result.total, result.page,
                                            result.pageSize, items);
    callback(JsonResponse::success(data));
}

void AttendanceController::createAttendance(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto json = req->getJsonObject();
    if (!json)
    {
        callback(JsonResponse::badRequest("无效的JSON数据"));
        return;
    }

    std::string studentId = (*json)["student_id"].asString();
    std::string date = (*json)["date"].asString();
    std::string status = (*json)["status"].asString();
    std::string remark;
    if (json->isMember("remark"))
    {
        remark = (*json)["remark"].asString();
    }

    if (studentId.empty() || date.empty() || status.empty())
    {
        callback(JsonResponse::badRequest("缺少必填字段"));
        return;
    }

    auto [success, att] = AttendanceService::getInstance().createAttendance(
        studentId, date, status, remark);

    if (success)
    {
        callback(JsonResponse::created(att.toJson(), "考勤记录创建成功"));
    }
    else
    {
        callback(JsonResponse::badRequest("创建失败，请检查学号是否存在或状态是否有效"));
    }
}

void AttendanceController::batchCreateAttendances(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto json = req->getJsonObject();
    if (!json)
    {
        callback(JsonResponse::badRequest("无效的JSON数据"));
        return;
    }

    std::string date = (*json)["date"].asString();
    if (date.empty())
    {
        callback(JsonResponse::badRequest("日期不能为空"));
        return;
    }

    if (!json->isMember("records") || !(*json)["records"].isArray())
    {
        callback(JsonResponse::badRequest("records字段必须是数组"));
        return;
    }

    std::vector<std::pair<std::string, std::string>> records;
    for (const auto &record : (*json)["records"])
    {
        std::string studentId = record["student_id"].asString();
        std::string status = record["status"].asString();
        if (!studentId.empty() && !status.empty())
        {
            records.emplace_back(studentId, status);
        }
    }

    int count = AttendanceService::getInstance().batchCreateAttendances(date, records);

    Json::Value data;
    data["created_count"] = count;
    callback(JsonResponse::created(data, "批量创建成功"));
}

void AttendanceController::getAttendance(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    int id) const
{
    auto att = AttendanceService::getInstance().getAttendance(id);

    if (att)
    {
        callback(JsonResponse::success(att->toJson()));
    }
    else
    {
        callback(JsonResponse::notFound("考勤记录不存在"));
    }
}

void AttendanceController::updateAttendance(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    int id) const
{
    auto json = req->getJsonObject();
    if (!json)
    {
        callback(JsonResponse::badRequest("无效的JSON数据"));
        return;
    }

    std::string status;
    std::string remark;

    if (json->isMember("status"))
    {
        status = (*json)["status"].asString();
    }
    if (json->isMember("remark"))
    {
        remark = (*json)["remark"].asString();
    }

    auto [success, message] = AttendanceService::getInstance().updateAttendance(
        id, status, remark);

    if (success)
    {
        auto att = AttendanceService::getInstance().getAttendance(id);
        if (att)
        {
            callback(JsonResponse::success(att->toJson(), message));
        }
        else
        {
            callback(JsonResponse::serverError("更新后无法获取记录"));
        }
    }
    else
    {
        callback(JsonResponse::notFound(message));
    }
}

void AttendanceController::deleteAttendance(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    int id) const
{
    if (AttendanceService::getInstance().deleteAttendance(id))
    {
        callback(JsonResponse::noContent());
    }
    else
    {
        callback(JsonResponse::notFound("考勤记录不存在"));
    }
}

}  // namespace v1
}  // namespace api


