#pragma once
#include "external.hpp"

struct Headers : public std::map<std::string, std::string>
{
    Headers();
    Headers(const std::string &contentType, const std::string &contentLength = "0", const std::string &location = "",
            const std::string &allow = "");
};