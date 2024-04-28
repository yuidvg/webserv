#pragma once
#include "external.hpp"
#include <CgiResponse.hpp>
#include <HttpRequest.hpp>

struct Parsed
{
    std::queue<HttpRequest> httpRequests;
    std::queue<CgiResponse> cgiResponses;
};
