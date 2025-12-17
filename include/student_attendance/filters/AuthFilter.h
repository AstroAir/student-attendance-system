#pragma once

#include <drogon/HttpFilter.h>

namespace student_attendance
{
namespace filters
{

class AuthFilter : public drogon::HttpFilter<AuthFilter>
{
public:
    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&fcb,
                  drogon::FilterChainCallback &&fccb) override;
};

}  // namespace filters
}  // namespace student_attendance
