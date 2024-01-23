#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include "external.hpp"

struct Server
{
    const std::string name;
    const size_t port;
    const std::map<int,std::string> errorPages;
    const size_t clientMaxBodySize;
    const std::vector<const Location> locations;
    Server();
    Server(const std::string name, const size_t port, const std::map<int,std::string> errorPages,
           const size_t clientMaxBodySize, const std::vector<const Location> locations);
};

typedef std::vector<const Server> Servers;

#endif
