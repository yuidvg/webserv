#pragma once
#include "Headers.hpp"
#include "external.hpp"

struct HttpResponse
{
    const unsigned int statusCode;
    const Headers headers;
    const std::string body;

    HttpResponse();
    HttpResponse(const unsigned int statusCode, const Headers headers = Headers(), const std::string body = "");
};
