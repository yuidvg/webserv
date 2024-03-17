#pragma once
#include "external.hpp"

struct CgiRequest
{
    const std::map<std::string, std::string> envs;
    const std::string scriptPath;
    const std::string body;

    CgiRequest(const std::map<std::string, std::string> envs, const std::string scriptPath, const std::string body)
        : envs(envs), scriptPath(scriptPath), body(body){};
    ~CgiRequest(){};
    CgiRequest() : envs(), scriptPath(), body(){};
};
