#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "../httpRequest/parsedRequest.hpp"
#include "../utils/Headers.hpp"
#include "../utils/utils.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>

struct HttpResponse
{
    const unsigned int statusCode;
    const Headers headers;
    const std::string body;

    HttpResponse();
    HttpResponse(const unsigned int statusCode, const Headers headers = Headers(), const std::string body = "");
};
struct ParsedRequest;

HttpResponse response(const ParsedRequest request, const Server server);
std::string makeResponseMessage(const HttpResponse &response);

#endif
