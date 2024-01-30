#pragma once
#include "Headers.hpp"

struct HttpRequest
{
    const std::string method;
    const std::string target;
    const std::string version;
    const Headers headers;
    const std::string body;
    const std::string host;

    HttpRequest(){};
    HttpRequest(const std::string &m, const std::string &t, const std::string &v, const Headers &h,
                const std::string &b, const std::string &host)
        : method(m), target(t), version(v), headers(h), body(b), host(host){};
};
