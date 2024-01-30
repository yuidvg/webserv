#pragma once

#include "external.hpp"

namespace
{
std::vector<std::string> initializeAllowedMethods()
{
    std::vector<std::string> allowedMethods;
    allowedMethods.push_back("GET");
    allowedMethods.push_back("POST");
    allowedMethods.push_back("DELETE");
    return allowedMethods;
}
} // namespace

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

    Location(const std::string path, const std::string root, const bool autoindex, const std::string index,
             const std::vector<std::string> allowMethods, const std::string cgiExtension, const std::string uploadPath,
             const std::map<int, std::string> redirect)
        : path(path), root(root), autoindex(autoindex), index(index), allowMethods(allowMethods),
          cgiExtension(cgiExtension), uploadPath(uploadPath), redirect(redirect){};
    Location()
        : path(""), root(""), autoindex(false), index(""),
          allowMethods(std::vector<std::string>(initializeAllowedMethods())), cgiExtension(""), uploadPath(""),
          redirect(std::map<int, std::string>()){};
};

typedef std::vector<Location> Locations;

