#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
    : statusCode(SUCCESS), host(""), method(""), target(""), version(""), headers(Headers()), body("")
{
}

HttpRequest::HttpRequest(const unsigned int statusCode, const std::string host, const std::string method,
                         const std::string target, const std::string version, const Headers headers,
                         const std::string body)
    : statusCode(statusCode), method(method), target(target), version(version), headers(headers), body(body), host(host)
{
}
