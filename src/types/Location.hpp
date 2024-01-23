#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "external.hpp"

struct Location
{
    const std::string path;
    const std::string root;
    const bool autoindex;
    const std::string index;
    const size_t clientMaxBodySize;
    const std::map<int, std::string> errorPages;
    std::vector<std::string> allowMethods; // 許可されるHTTPメソッド（GET, POST, DELETE）
    const std::string cgiExtension;
    const std::string uploadPath;
    const std::map<int, std::string> redirect;
    Location();
    Location(const std::string path, const std::string root, const bool autoindex, const std::string index,
             const size_t clientMaxBodySize, const std::map<int, std::string> errorPages,
             const std::vector<std::string> allowMethods, const std::string cgiExtension, const std::string uploadPath,
             const std::map<int, std::string> redirect);
};

#endif
