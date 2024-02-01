#pragma once
#include "../globals.hpp"
#include "Headers.hpp"
#include "external.hpp"

struct HttpResponse
{
    const unsigned int statusCode;
    const std::string body;
    const std::string contentType;
    const std::string location;
    const std::string allow;

    HttpResponse() : statusCode(0), body(""), contentType(""), location(""), allow("")
    {
    }
    HttpResponse(const unsigned int &statusCode, const std::string &body = "", const std::string &contentType = "",
                 const std::string &location = "", const std::string &allow = "")
        : statusCode(statusCode), body(body), contentType(contentType), location(location), allow(allow)
    {
    }
};
