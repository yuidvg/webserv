#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <cstring>
#include <iostream>
#include <map>

const char *CRLF = "\r\n";

struct HttpResponse
{
    const int statusCode;
    const std::map<std::string, std::string> headers;
    const std::string body;
};

#endif
