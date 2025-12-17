#include "student_attendance/controllers/StudentController.h"
#include "student_attendance/services/StudentService.h"
#include "student_attendance/utils/JsonResponse.h"

using namespace drogon;
using namespace student_attendance::services;
using namespace student_attendance::utils;

namespace api
{
namespace v1
{

void StudentController::getStudents(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    // Parse query parameters
    int page = 1;
    int pageSize = 20;
    std::string sortBy;
    std::string order = "asc";
    std::string className;
    std::string keyword;

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

    sortBy = req->getParameter("sort_by");
    auto orderParam = req->getParameter("order");
    if (!orderParam.empty())
    {
        order = orderParam;
    }
    className = req->getParameter("class");
    keyword = req->getParameter("keyword");

    auto result = StudentService::getInstance().getStudents(
        page, pageSize, sortBy, order, className, keyword);

    Json::Value items(Json::arrayValue);
    for (const auto &student : result.students)
    {
        items.append(student.toJson());
    }

    auto data = JsonResponse::paginatedData(result.total, result.page,
                                            result.pageSize, items);
    callback(JsonResponse::success(data));
}

void StudentController::createStudent(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto json = req->getJsonObject();
    if (!json)
    {
        callback(JsonResponse::badRequest("无效的JSON数据"));
        return;
    }

    student_attendance::models::Student student = student_attendance::models::Student::fromJson(*json);

    auto [success, message] = StudentService::getInstance().createStudent(student);

    if (success)
    {
        callback(JsonResponse::created(student.toJson(), message));
    }
    else
    {
        if (message.find("已存在") != std::string::npos)
        {
            callback(JsonResponse::conflict(message));
        }
        else
        {
            callback(JsonResponse::badRequest(message));
        }
    }
}

void StudentController::getStudent(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string &studentId) const
{
    auto student = StudentService::getInstance().getStudent(studentId);

    if (student)
    {
        callback(JsonResponse::success(student->toJson()));
    }
    else
    {
        callback(JsonResponse::notFound("学生不存在"));
    }
}

void StudentController::updateStudent(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string &studentId) const
{
    auto json = req->getJsonObject();
    if (!json)
    {
        callback(JsonResponse::badRequest("无效的JSON数据"));
        return;
    }

    std::string name;
    std::string className;

    if (json->isMember("name"))
    {
        name = (*json)["name"].asString();
    }
    if (json->isMember("class"))
    {
        className = (*json)["class"].asString();
    }

    auto [success, message] = StudentService::getInstance().updateStudent(
        studentId, name, className);

    if (success)
    {
        auto student = StudentService::getInstance().getStudent(studentId);
        if (student)
        {
            callback(JsonResponse::success(student->toJson(), message));
        }
        else
        {
            callback(JsonResponse::serverError("更新后无法获取学生信息"));
        }
    }
    else
    {
        callback(JsonResponse::notFound(message));
    }
}

void StudentController::deleteStudent(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string &studentId) const
{
    if (StudentService::getInstance().deleteStudent(studentId))
    {
        callback(JsonResponse::noContent());
    }
    else
    {
        callback(JsonResponse::notFound("学生不存在"));
    }
}

}  // namespace v1
}  // namespace api


