#pragma once
#include "external.hpp"

typedef std::map<const std::string, const std::string> StringMap;

struct CgiRequest
{
    const int destinationSd;
    const StringMap envs;
    const std::string scriptPath;
    const std::string body;

    CgiRequest(const int destinationSd, const StringMap envs, const std::string scriptPath, const std::string body)
        : destinationSd(destinationSd), envs(envs), scriptPath(scriptPath), body(body){};
    ~CgiRequest(){};
    CgiRequest() : destinationSd(-1), envs(), scriptPath(), body(){};
};
