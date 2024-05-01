#pragma once
#include "HttpRequest.hpp"
#include "Socket.hpp"
#include "external.hpp"

typedef std::map<const std::string, const std::string> StringMap;

struct CgiRequest
{
    const Socket cgiSocket;
    const HttpRequest httpRequest;
    const StringMap envs;
    const std::string scriptPath;
    const std::string body;

    CgiRequest(const Socket cgiSocket, const HttpRequest httpRequest, const StringMap envs,
               const std::string scriptPath, const std::string body)
        : cgiSocket(cgiSocket), httpRequest(httpRequest), envs(envs), scriptPath(scriptPath), body(body){};
    ~CgiRequest(){};
    CgiRequest() : cgiSocket(), httpRequest(), envs(), scriptPath(), body(){};
};
