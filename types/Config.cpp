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

std::vector<std::string> initializeAllowedMethods() {
    std::vector<std::string> methods;
    methods.push_back("GET");
    methods.push_back("POST");
    methods.push_back("DELETE");
    return methods;
}


Location::Location() 
    : path(""),
      root(""),
      autoindex(false),
      index("index.html"),
      clientMaxBodySize(1048576),
      errorPages(std::map<int, std::string>()),
      allowMethods(std::vector<std::string>(initializeAllowedMethods())),
      cgiExtension(""),
      uploadPath(""),
      redirect(std::map<int, std::string>())
{
}
Server::Server()
    : name(""), 
      port(80), 
      errorPages(std::map<int, std::string>()), 
      clientMaxBodySize(1048576), 
      locations(std::vector<Location>())
{
}

Server::Server(const std::string name, const size_t port, const std::map<int, std::string> errorPages,
           const size_t clientMaxBodySize, const std::vector<Location> locations)
           : name(name), port(port), errorPages(errorPages), clientMaxBodySize(clientMaxBodySize), locations(locations)
{
}