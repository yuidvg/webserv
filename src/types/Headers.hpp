#pragma once
#include "external.hpp"

struct Headers
{
    std::string host;
    

    // Cannonical
    Headers(){};
    ~Headers(){};
    Headers(const Headers &other)
        : host(other.host), contentType(other.contentType), contentLength(other.contentLength),
          location(other.location), allow(other.allow), transferEncoding(other.transferEncoding){};
    Headers &operator=(const Headers &other)
    {
        if (this != &other)
        {
            host = other.host;
            contentType = other.contentType;
            contentLength = other.contentLength;
            location = other.location;
            allow = other.allow;
            transferEncoding = other.transferEncoding;
        }
        return *this;
    };

    Headers(const std::string &host, const std::string &contentType, const std::string &contentLength = "0",
            const std::string &location = "", const std::string &allow = "", const std::string &transferEncoding = "")
        : host(host), contentType(contentType), contentLength(contentLength), location(location), allow(allow),
          transferEncoding(transferEncoding){};
};
