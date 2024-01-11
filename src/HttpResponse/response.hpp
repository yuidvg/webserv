#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "../httpRequest/parseRequest.hpp"
#include <cstring>
#include <iostream>
#include <map>

struct HttpResponse
{
    const int statusCode;
    const std::map<std::string, std::string> headers;
    const std::string body;
};

#endif
