#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "../httpRequest/parseRequest.hpp"
#include "../utils/utils.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>

struct HttpResponse
{
    const unsigned int statusCode;
    const std::map<std::string, std::string> headers;
    const std::string body;

    HttpResponse(const unsigned int statusCode, const std::map<std::string, std::string> headers,
                 const std::string body)
        : statusCode(statusCode), headers(headers), body(body)
    {
    }
};

HttpResponse response(const ParsedRequest request, const Server server);
std::string makeResponseMessage(const HttpResponse &response);

#endif
