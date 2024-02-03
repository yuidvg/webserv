#pragma once
#include "../webserv.hpp"

namespace
{
struct CgiResponse
{
    const std::string body;
    const std::string contentType;
    const std::string location;
    const unsigned int statusCode;
    const Headers headers;

    CgiResponse(int fd, std::string ) : body(""), contentType(""), location(""), statusCode(0), headers()
    {
        ssize_t n;
        char buffer[1024];
        std::string response;
        while ((n = read(fd, buffer, 1024)) > 0)
        {
            response += buffer;
        }
    };
    CgiResponse(const std::string &body, const std::string &contentType, const std::string &location,
                const unsigned int &statusCode, const Headers &headers)
        : body(body), contentType(contentType), location(location), statusCode(statusCode), headers(headers){};
};
} // namespace
