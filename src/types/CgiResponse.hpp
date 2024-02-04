#pragma once

#include "Headers.hpp"
#include <string>

struct CgiResponse
{
    const std::string contentType;
    const std::string location;
    const unsigned int status;
    const Headers otherHeaders;
    const std::string body;

    CgiResponse(const std::string &contentType, const std::string &location, const unsigned int status,
                const Headers &otherHeaders, const std::string &body)
        : contentType(contentType), location(location), status(status), otherHeaders(otherHeaders), body(body)
    {
    }
    CgiResponse() : contentType(""), location(""), status(0), otherHeaders(""), body("")
    {
    }
};
