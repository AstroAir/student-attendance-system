#include "student_attendance/filters/AuthFilter.h"

#include "student_attendance/utils/JsonResponse.h"

using namespace student_attendance::utils;

namespace student_attendance
{
namespace filters
{

void AuthFilter::doFilter(const drogon::HttpRequestPtr &req,
                          drogon::FilterCallback &&fcb,
                          drogon::FilterChainCallback &&fccb)
{
    if (req->path().rfind("/api/v1/auth/", 0) == 0)
    {
        fccb();
        return;
    }

    if (!req->session())
    {
        fcb(JsonResponse::serverError("Session未启用"));
        return;
    }

    auto userIdOpt = req->session()->getOptional<int>("user_id");
    if (!userIdOpt)
    {
        fcb(JsonResponse::unauthorized("未登录"));
        return;
    }

    fccb();
}

}  // namespace filters
}  // namespace student_attendance
