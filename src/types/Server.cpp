#include "Server.hpp"
#include "utils/.hpp"

Server::Server()
    : name(""), port(80), errorPages(std::map<int, std::string>()), clientMaxBodySize(1048576),
      locations(std::vector<const Location>())
{
}

Server::Server(const std::string name, const size_t port, const std::map<int, std::string> errorPages,
               const size_t clientMaxBodySize, const std::vector<const Location> locations)
    : name(name), port(port), errorPages(errorPages), clientMaxBodySize(clientMaxBodySize), locations(locations)
{
}

Location matchedLocation(const std::string &target, const std::vector<const Location> &locations)
{
    unsigned int matchedIndex = 0;
    for (unsigned int i = 1; i < locations.size(); i++)
    {
        if (utils::lengthOfPrefixMatch(target, locations[i].path) >
            utils::lengthOfPrefixMatch(target, locations[matchedIndex].path))
            matchedIndex = i;
    }
    return locations[matchedIndex];
}
