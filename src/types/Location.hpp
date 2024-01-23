#pragma once

#include "external.hpp"

struct Location
{
    const std::string path;
    const std::string root;
    const bool autoindex;
    const std::string index;
    std::vector<std::string> allowMethods; // 許可されるHTTPメソッド（GET, POST, DELETE）
    const std::string cgiExtension;
    const std::string uploadPath;
    const std::map<int, std::string> redirect;
    Location();
    Location(const std::string path, const std::string root, const bool autoindex, const std::string index,
             const std::vector<std::string> allowMethods, const std::string cgiExtension, const std::string uploadPath,
             const std::map<int, std::string> redirect);
};
