#pragma once
#include "Headers.hpp"

struct HttpRequest
{
    const int sd;
    const size_t serverPort;
    const std::string clientIp;
    const std::string host;
    const std::string method;
    const std::string target;
    const Headers headers;
    const std::string body;

    HttpRequest()
        : sd(0), serverPort(0), clientIp(""), host(""), method(""), target(""), headers(Headers()), body(""){};
    HttpRequest(const int sd, const size_t port, const std::string &clientIp, const std::string &host,
                const std::string &m, const std::string &t, const Headers &h, const std::string &b)
        : sd(sd), serverPort(port), clientIp(clientIp), host(host), method(m), target(t), headers(h), body(b){};
};
