#include "student_attendance/controllers/ClassController.h"
#include "student_attendance/models/DataStore.h"
#include "student_attendance/utils/JsonResponse.h"

using namespace drogon;
using namespace student_attendance::models;
using namespace student_attendance::utils;

namespace api
{
namespace v1
{

void ClassController::getClasses(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto &dataStore = DataStore::getInstance();
    auto classes = dataStore.getAllClasses();

    Json::Value data(Json::arrayValue);
    for (const auto &className : classes)
    {
        Json::Value classInfo;
        classInfo["name"] = className;
        classInfo["student_count"] = dataStore.getClassStudentCount(className);
        data.append(classInfo);
    }

    callback(JsonResponse::success(data));
}

void ClassController::getClassStudents(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string &className) const
{
    auto &dataStore = DataStore::getInstance();

    // URL decode the class name (handle Chinese characters)
    std::string decodedClassName = className;

    auto students = dataStore.getStudentsByClass(decodedClassName);

    if (students.empty())
    {
        // Check if class exists
        auto allClasses = dataStore.getAllClasses();
        bool classExists = false;
        for (const auto &cls : allClasses)
        {
            if (cls == decodedClassName)
            {
                classExists = true;
                break;
            }
        }

        if (!classExists)
        {
            callback(JsonResponse::notFound("班级不存在"));
            return;
        }
    }

    Json::Value data;
    data["class"] = decodedClassName;

    Json::Value studentArray(Json::arrayValue);
    for (const auto &student : students)
    {
        studentArray.append(student.toBasicJson());
    }
    data["students"] = studentArray;

    callback(JsonResponse::success(data));
}

}  // namespace v1
}  // namespace api


