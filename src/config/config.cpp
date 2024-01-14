#include "config.hpp"

Location::Location(const std::string path, const std::string root, const bool autoindex, const std::string index,
                   const size_t clientMaxBodySize, const std::map<int, std::string> errorPages,
                   const std::vector<std::string> allowMethods, const std::string cgiExtension,
                   const std::string uploadPath, const std::map<int, std::string> redirect)
    : path(path), root(root), autoindex(autoindex), index(index), clientMaxBodySize(clientMaxBodySize),
      errorPages(errorPages), allowMethods(allowMethods), cgiExtension(cgiExtension), uploadPath(uploadPath),
      redirect(redirect)
{
}

Location::Location() : autoindex(false), clientMaxBodySize(0)
{
}

Server::Server() : port(80), clientMaxBodySize(0), autoindex(false)
{
}
