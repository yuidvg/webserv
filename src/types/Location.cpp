#include "Location.hpp"

Location::Location(const std::string path, const std::string root, const bool autoindex, const std::string index,
             const std::vector<std::string> allowMethods, const std::string cgiExtension, const std::string uploadPath,
             const std::map<int, std::string> redirect)
    : path(path), root(root), autoindex(autoindex), index(index),
      allowMethods(allowMethods), cgiExtension(cgiExtension), uploadPath(uploadPath),
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
    : path(""), root(""), autoindex(false), index(""),
      allowMethods(std::vector<std::string>(initializeAllowedMethods())),
      cgiExtension(""), uploadPath(""), redirect(std::map<int, std::string>())
{
}
