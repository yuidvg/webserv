#pragma once
#include "../globals.hpp"
#include "Headers.hpp"
#include "external.hpp"

struct HttpResponse
{
    const int destinationSd;
    const unsigned int statusCode;
    const std::string body;
    const std::string contentType;
    const std::string location;
    const std::string allow;

    HttpResponse() : destinationSd(-1), statusCode(0), body(""), contentType(""), location(""), allow("")
    {
    }
    HttpResponse(const int sd, const unsigned int statusCode, const std::string &body = "",
                 const std::string &contentType = "", const std::string &location = "", const std::string &allow = "")
        : destinationSd(sd), statusCode(statusCode), body(body), contentType(contentType), location(location),
          allow(allow)
    {
    }
};
