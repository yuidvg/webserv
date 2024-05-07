#pragma once
#include "Headers.hpp"
#include "Socket.hpp"

struct HttpRequest
{
    const Socket socket;
    const std::string host;
    const std::string method;
    const std::string target;
    const Headers headers;
    const std::string body;

    HttpRequest() : socket(Socket()), host(""), method(""), target(""), headers(Headers()), body(""){};
    HttpRequest(const Socket &socket, const std::string &host = "", const std::string &method = "",
                const std::string &target = "", const Headers &headers = Headers(), const std::string &body = "")
        : socket(socket), host(host), method(method), target(target), headers(headers), body(body){};
};
