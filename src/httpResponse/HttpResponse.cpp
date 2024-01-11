#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : statusCode(SUCCESS), headers(Headers()), body("")
{
}

HttpResponse::HttpResponse(const unsigned int statusCode, const Headers headers, const std::string body)
    : statusCode(statusCode), headers(headers), body(body)
{
}
