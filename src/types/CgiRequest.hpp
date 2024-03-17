#pragma once
#include "external.hpp"

struct CgiRequest
{
    const std::map<std::string, std::string> envs;
    const std::string target;
    const std::string body;

    CgiRequest(const std::map<std::string, std::string> envs, const std::string target, const std::string body)
        : envs(envs), target(target), body(body){};
    ~CgiRequest(){};
    CgiRequest() : envs(), target(), body(){};
};
