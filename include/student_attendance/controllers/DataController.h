#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

namespace api
{
namespace v1
{

class DataController : public drogon::HttpController<DataController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(DataController::exportData, "/api/v1/data/export", drogon::Get, "student_attendance::filters::AuthFilter");
    ADD_METHOD_TO(DataController::importData, "/api/v1/data/import", drogon::Post, "student_attendance::filters::AuthFilter");
    METHOD_LIST_END

    void exportData(const drogon::HttpRequestPtr &req,
                    std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void importData(const drogon::HttpRequestPtr &req,
                    std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;
};

}  // namespace v1
}  // namespace api

