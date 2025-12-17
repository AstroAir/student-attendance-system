#pragma once

#include <drogon/HttpResponse.h>
#include <json/json.h>
#include <string>

namespace student_attendance
{
namespace utils
{

class JsonResponse
{
public:
    static drogon::HttpResponsePtr success(const Json::Value &data,
                                           const std::string &message = "success")
    {
        Json::Value response;
        response["code"] = 200;
        response["message"] = message;
        response["data"] = data;
        auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(drogon::k200OK);
        return resp;
    }

    static drogon::HttpResponsePtr created(const Json::Value &data,
                                           const std::string &message = "创建成功")
    {
        Json::Value response;
        response["code"] = 201;
        response["message"] = message;
        response["data"] = data;
        auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(drogon::k201Created);
        return resp;
    }

    static drogon::HttpResponsePtr noContent()
    {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k204NoContent);
        return resp;
    }

    static drogon::HttpResponsePtr badRequest(const std::string &message,
                                              const Json::Value &errors = Json::Value())
    {
        Json::Value response;
        response["code"] = 400;
        response["message"] = message;
        if (!errors.isNull())
        {
            response["errors"] = errors;
        }
        auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(drogon::k400BadRequest);
        return resp;
    }

    static drogon::HttpResponsePtr unauthorized(const std::string &message)
    {
        Json::Value response;
        response["code"] = 401;
        response["message"] = message;
        auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(drogon::k401Unauthorized);
        return resp;
    }

    static drogon::HttpResponsePtr notFound(const std::string &message)
    {
        Json::Value response;
        response["code"] = 404;
        response["message"] = message;
        auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(drogon::k404NotFound);
        return resp;
    }

    static drogon::HttpResponsePtr conflict(const std::string &message)
    {
        Json::Value response;
        response["code"] = 409;
        response["message"] = message;
        auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(drogon::k409Conflict);
        return resp;
    }

    static drogon::HttpResponsePtr serverError(const std::string &message)
    {
        Json::Value response;
        response["code"] = 500;
        response["message"] = message;
        auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(drogon::k500InternalServerError);
        return resp;
    }

    static Json::Value paginatedData(int total, int page, int pageSize,
                                     const Json::Value &items)
    {
        Json::Value data;
        data["total"] = total;
        data["page"] = page;
        data["page_size"] = pageSize;
        data["items"] = items;
        return data;
    }

    static Json::Value periodData(const std::string &startDate,
                                  const std::string &endDate)
    {
        Json::Value period;
        period["start_date"] = startDate;
        period["end_date"] = endDate;
        return period;
    }
};

}  // namespace utils
}  // namespace student_attendance

