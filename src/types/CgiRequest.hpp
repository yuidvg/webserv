#pragma once
#include "external.hpp"

struct CgiRequest
{
    const int destinationSd;
    const StringMap envs;
    const std::string scriptPath;
    const std::string body;

    CgiRequest(const int destinationSd, const StringMap envs, const std::string scriptPath, const std::string body)
        : destinationSd(destinationSd), envs(envs), scriptPath(scriptPath), body(body){};
    ~CgiRequest(){};
    CgiRequest() : destinationSd(destinationSd), envs(), scriptPath(), body(){};
};
