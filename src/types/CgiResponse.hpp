#pragma once
#include "Headers.hpp"
#include "HttpRequest.hpp"
#include "Socket.hpp"
#include "external.hpp"

struct CgiResponse
{
    const Socket cgiSocket;
    const HttpRequest httpRequest;
    const std::string contentType;
    const std::string location;
    const unsigned int status;
    const Headers otherHeaders;
    const std::string body;

    CgiResponse(const Socket cgiSocket, const HttpRequest &httpRequest, const std::string &contentType,
                const std::string &location, const unsigned int status, const Headers &otherHeaders,
                const std::string &body)
        : cgiSocket(cgiSocket), httpRequest(httpRequest), contentType(contentType), location(location), status(status),
          otherHeaders(otherHeaders), body(body)
    {
    }
    CgiResponse()
        : cgiSocket(), httpRequest(HttpRequest()), contentType(""), location(""), status(0), otherHeaders(""), body("")
    {
    }
};
