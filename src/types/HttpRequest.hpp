#pragma once
#include "Headers.hpp"
#include "Socket.hpp"

struct HttpRequest
{
    const Socket socket;
    const std::string host;
    const std::string method;
    const std::string target;
    const std::string contentType;
    const std::string contentLength;
    const std::string location;
    const std::string allow;
    const std::string transferEncoding;
    const std::string body;

    HttpRequest()
        : socket(Socket()), host(""), method(""), target(""), contentType(""), contentLength(""), location(""),
          allow(""), transferEncoding(""), body(""){};
    HttpRequest(const Socket &socket, const std::string &host, const std::string &method, const std::string &target,
                const std::string &contentType, const std::string &contentLength, const std::string &location,
                const std::string &allow, const std::string &transferEncoding, const std::string &body)
        : socket(socket), host(host), method(method), target(target), contentType(contentType),
          contentLength(contentLength), location(location), allow(allow), transferEncoding(transferEncoding),
          body(body){};
    HttpRequest(const Socket &socket)
        : socket(socket), host(""), method(""), target(""), contentType(""), contentLength(""), location(""), allow(""),
          transferEncoding(""), body(""){};
};
