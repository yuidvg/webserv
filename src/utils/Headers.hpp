#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <map>
#include <string>

struct Headers : public std::map<std::string, std::string>
{
    Headers();
    Headers(const std::string contentType, const std::string contentLength = "", const std::string location = "");
};

#endif
