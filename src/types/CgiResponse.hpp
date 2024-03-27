#pragma once
#include "Headers.hpp"
#include "external.hpp"

struct CgiResponse
{
    const int cgiSd;
    const std::string contentType;
    const std::string location;
    const unsigned int status;
    const Headers otherHeaders;
    const std::string body;

    CgiResponse(const int cgiSd, const std::string &contentType, const std::string &location, const unsigned int status,
                const Headers &otherHeaders, const std::string &body)
        : cgiSd(cgiSd), contentType(contentType), location(location), status(status), otherHeaders(otherHeaders),
          body(body)
    {
    }
    CgiResponse() : cgiSd(-1), contentType(""), location(""), status(0), otherHeaders(""), body("")
    {
    }
};
