#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

namespace api
{
namespace v1
{

class ClassController : public drogon::HttpController<ClassController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ClassController::getClasses, "/api/v1/classes", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(ClassController::getClassStudents, "/api/v1/classes/{class_name}/students", drogon::Get, "student_attendance::filters::AuthFilter");
    METHOD_LIST_END

    void getClasses(const drogon::HttpRequestPtr &req,
                    std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void getClassStudents(const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                          const std::string &className) const;
};

}  // namespace v1
}  // namespace api

