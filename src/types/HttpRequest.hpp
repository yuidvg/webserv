#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "Headers.hpp"

struct HttpRequest
{
    const std::string method;
    const std::string target;
    const std::string host;
    const std::string version;
    const Headers headers;
    const std::string body;

    HttpRequest();
    HttpRequest(const std::string &m, const std::string &t, const std::string &ho, const std::string &v,
                const Headers &h, const std::string &b);
};

#endif
