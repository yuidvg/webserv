#include "Location.hpp"

Location::Location(const std::string path, const std::string root, const bool autoindex, const std::string index,
                   const size_t clientMaxBodySize, const std::map<int, std::string> errorPages,
                   const std::vector<std::string> allowMethods, const std::string cgiExtension,
                   const std::string uploadPath, const std::map<int, std::string> redirect)
    : path(path), root(root), autoindex(autoindex), index(index), clientMaxBodySize(clientMaxBodySize),
      errorPages(errorPages), allowMethods(allowMethods), cgiExtension(cgiExtension), uploadPath(uploadPath),
      redirect(redirect)
{
}

static std::vector<std::string> initializeAllowedMethods()
{
    std::vector<std::string> methods;
    methods.push_back("GET");
    methods.push_back("POST");
    methods.push_back("DELETE");
    return methods;
}

Location::Location()
    : path(""), root(""), autoindex(false), index("index.html"), clientMaxBodySize(1048576),
      errorPages(std::map<int, std::string>()), allowMethods(std::vector<std::string>(initializeAllowedMethods())),
      cgiExtension(""), uploadPath(""), redirect(std::map<int, std::string>())
{
}
