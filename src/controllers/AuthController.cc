#include "student_attendance/controllers/AuthController.h"

#include "student_attendance/services/AuthService.h"
#include "student_attendance/utils/JsonResponse.h"

using namespace student_attendance::services;
using namespace student_attendance::utils;

namespace api
{
namespace v1
{

void AuthController::login(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) const
{
    auto json = req->getJsonObject();
    if (!json)
    {
        callback(JsonResponse::badRequest("无效的JSON数据"));
        return;
    }

    if (!json->isMember("username") || !json->isMember("password"))
    {
        callback(JsonResponse::badRequest("缺少必要字段"));
        return;
    }

    auto username = (*json)["username"].asString();
    auto password = (*json)["password"].asString();

    auto userOpt = AuthService::getInstance().authenticate(username, password);
    if (!userOpt)
    {
        callback(JsonResponse::unauthorized("用户名或密码错误"));
        return;
    }

    if (!req->session())
    {
        callback(JsonResponse::serverError("Session未启用"));
        return;
    }

    req->session()->insert("user_id", userOpt->id);
    req->session()->insert("username", userOpt->username);
    req->session()->insert("role", userOpt->role);

    callback(JsonResponse::success(userOpt->toJson(), "登录成功"));
}

void AuthController::logout(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) const
{
    if (req->session())
    {
        req->session()->erase("user_id");
        req->session()->erase("username");
        req->session()->erase("role");
    }

    callback(JsonResponse::success(Json::Value(), "退出成功"));
}

void AuthController::me(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) const
{
    if (!req->session())
    {
        callback(JsonResponse::serverError("Session未启用"));
        return;
    }

    auto userIdOpt = req->session()->getOptional<int>("user_id");
    auto usernameOpt = req->session()->getOptional<std::string>("username");
    auto roleOpt = req->session()->getOptional<std::string>("role");

    if (!userIdOpt || !usernameOpt || !roleOpt)
    {
        callback(JsonResponse::unauthorized("未登录"));
        return;
    }

    Json::Value user;
    user["id"] = *userIdOpt;
    user["username"] = *usernameOpt;
    user["role"] = *roleOpt;

    callback(JsonResponse::success(user));
}

}  // namespace v1
}  // namespace api
