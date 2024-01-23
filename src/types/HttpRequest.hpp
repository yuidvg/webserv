#pragma once
#include "Headers.hpp"

struct HttpRequest
{
    const unsigned int statusCode;
    const std::string host;
    const std::string method;
    const std::string target;
    const std::string version;
    const Headers headers;
    const std::string body;

    HttpRequest();
    HttpRequest(const unsigned int statusCode, const std::string host = "", const std::string method = "",
                const std::string target = "", const std::string version = "", const Headers headers = Headers(),
                const std::string body = "");
};
