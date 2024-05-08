#pragma once
#include "Headers.hpp"
#include "Socket.hpp"

struct HttpRequest
{
    Socket socket;
    std::string host;
    std::string method;
    std::string target;
    Headers headers;
    std::string body;

    // Cannonical form

    HttpRequest() : socket(Socket()), host(""), method(""), target(""), headers(Headers()), body(""){};
    ~HttpRequest(){};
    HttpRequest(const HttpRequest &other)
        : socket(other.socket), host(other.host), method(other.method), target(other.target), headers(other.headers),
          body(other.body){};
    HttpRequest &operator=(const HttpRequest &other)
    {
        if (this != &other)
        {
            socket = other.socket;
            host = other.host;
            method = other.method;
            target = other.target;
            headers = other.headers;
            body = other.body;
        }
        return *this;
    }

    HttpRequest(const Socket &socket, const std::string &host = "", const std::string &method = "",
                const std::string &target = "", const Headers &headers = Headers(), const std::string &body = "")
        : socket(socket), host(host), method(method), target(target), headers(headers), body(body){};
};
