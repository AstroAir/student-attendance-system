#include "student_attendance/controllers/DataController.h"
#include "student_attendance/models/DataStore.h"
#include "student_attendance/models/Student.h"
#include "student_attendance/models/Attendance.h"
#include "student_attendance/utils/JsonResponse.h"
#include <sstream>

using namespace drogon;
using namespace student_attendance::models;
using namespace student_attendance::utils;

namespace api
{
namespace v1
{

void DataController::exportData(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    std::string type = req->getParameter("type");
    std::string format = req->getParameter("format");

    if (type.empty())
    {
        callback(JsonResponse::badRequest("type为必填参数"));
        return;
    }

    if (format.empty())
    {
        format = "json";
    }

    auto &dataStore = DataStore::getInstance();

    if (format == "json")
    {
        Json::Value data;

        if (type == "students" || type == "all")
        {
            Json::Value students(Json::arrayValue);
            for (const auto &student : dataStore.getAllStudents())
            {
                students.append(student.toJson());
            }
            data["students"] = students;
        }

        if (type == "attendances" || type == "all")
        {
            Json::Value attendances(Json::arrayValue);
            for (const auto &att : dataStore.getAllAttendances())
            {
                attendances.append(att.toJson());
            }
            data["attendances"] = attendances;
        }

        auto resp = HttpResponse::newHttpJsonResponse(data);
        resp->addHeader("Content-Disposition",
                       "attachment; filename=\"export.json\"");
        callback(resp);
    }
    else if (format == "csv")
    {
        std::ostringstream csv;

        if (type == "students")
        {
            csv << "student_id,name,class\n";
            for (const auto &student : dataStore.getAllStudents())
            {
                csv << student.studentId << ","
                    << student.name << ","
                    << student.className << "\n";
            }
        }
        else if (type == "attendances")
        {
            csv << "id,student_id,name,class,date,status,remark\n";
            for (const auto &att : dataStore.getAllAttendances())
            {
                csv << att.id << ","
                    << att.studentId << ","
                    << att.name << ","
                    << att.className << ","
                    << att.date << ","
                    << att.status << ","
                    << att.remark << "\n";
            }
        }
        else
        {
            callback(JsonResponse::badRequest("CSV格式不支持导出all类型"));
            return;
        }

        auto resp = HttpResponse::newHttpResponse();
        resp->setBody(csv.str());
        resp->setContentTypeCode(CT_TEXT_PLAIN);
        resp->addHeader("Content-Disposition",
                       "attachment; filename=\"export.csv\"");
        callback(resp);
    }
    else
    {
        callback(JsonResponse::badRequest("不支持的导出格式"));
    }
}

void DataController::importData(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto &dataStore = DataStore::getInstance();

    // Try to get JSON body first
    auto json = req->getJsonObject();
    if (json)
    {
        std::string type;
        if (json->isMember("type"))
        {
            type = (*json)["type"].asString();
        }

        int importedCount = 0;
        int skippedCount = 0;
        Json::Value errors(Json::arrayValue);

        if (type == "students" && json->isMember("data"))
        {
            const auto &data = (*json)["data"];
            for (Json::ArrayIndex i = 0; i < data.size(); ++i)
            {
                try
                {
                    Student student = Student::fromJson(data[i]);
                    if (dataStore.addStudent(student))
                    {
                        importedCount++;
                    }
                    else
                    {
                        skippedCount++;
                        Json::Value error;
                        error["line"] = static_cast<int>(i + 1);
                        error["message"] = "学号已存在";
                        errors.append(error);
                    }
                }
                catch (...)
                {
                    skippedCount++;
                    Json::Value error;
                    error["line"] = static_cast<int>(i + 1);
                    error["message"] = "数据格式错误";
                    errors.append(error);
                }
            }
        }
        else if (type == "attendances" && json->isMember("data"))
        {
            const auto &data = (*json)["data"];
            for (Json::ArrayIndex i = 0; i < data.size(); ++i)
            {
                try
                {
                    Attendance att = Attendance::fromJson(data[i]);
                    // Get student info
                    auto student = dataStore.getStudentById(att.studentId);
                    if (student)
                    {
                        att.name = student->name;
                        att.className = student->className;
                        dataStore.addAttendance(att);
                        importedCount++;
                    }
                    else
                    {
                        skippedCount++;
                        Json::Value error;
                        error["line"] = static_cast<int>(i + 1);
                        error["message"] = "学生不存在";
                        errors.append(error);
                    }
                }
                catch (...)
                {
                    skippedCount++;
                    Json::Value error;
                    error["line"] = static_cast<int>(i + 1);
                    error["message"] = "数据格式错误";
                    errors.append(error);
                }
            }
        }
        else
        {
            callback(JsonResponse::badRequest("无效的导入类型或数据"));
            return;
        }

        Json::Value result;
        result["imported_count"] = importedCount;
        result["skipped_count"] = skippedCount;
        result["errors"] = errors;

        callback(JsonResponse::success(result, "导入成功"));
    }
    else
    {
        // Handle multipart form data
        MultiPartParser fileParser;
        if (fileParser.parse(req) != 0)
        {
            callback(JsonResponse::badRequest("无法解析上传的文件"));
            return;
        }

        auto &files = fileParser.getFiles();
        if (files.empty())
        {
            callback(JsonResponse::badRequest("未找到上传的文件"));
            return;
        }

        auto &params = fileParser.getParameters();
        std::string type;
        auto it = params.find("type");
        if (it != params.end())
        {
            type = it->second;
        }

        if (type.empty())
        {
            callback(JsonResponse::badRequest("type参数为必填"));
            return;
        }

        // For now, return a placeholder response
        // Full file parsing would require more complex logic
        Json::Value result;
        result["imported_count"] = 0;
        result["skipped_count"] = 0;
        result["errors"] = Json::Value(Json::arrayValue);
        result["message"] = "文件上传功能已接收，请使用JSON格式导入";

        callback(JsonResponse::success(result, "导入完成"));
    }
}

}  // namespace v1
}  // namespace api


