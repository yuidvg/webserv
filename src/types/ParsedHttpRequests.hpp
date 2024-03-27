#pragma once

#include "HttpRequest.hpp"
#include "external.hpp"

struct ParsedHttpRequests
{
    const std::queue<const HttpRequest> httpRequests;
    const size_t size;

    ParsedHttpRequests(const std::queue<const HttpRequest> httpRequests, const size_t size)
        : httpRequests(httpRequests), size(size)
    {
    }
};
