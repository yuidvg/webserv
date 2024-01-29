#pragma once

#include "Location.hpp"
#include "external.hpp"

namespace
{
unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern)
{
    if (string.find(pattern) == 0)
        return pattern.length();
    else
        return 0;
}
} // namespace

struct Server
{
    const std::string name;
    const size_t port;
    const std::map<const int, const std::string> errorPages;
    const size_t clientMaxBodySize;
    const std::vector<const Location> locations;

    Server()
        : name(""), port(80), errorPages(std::map<const int, const std::string>()), clientMaxBodySize(1048576),
          locations(std::vector<const Location>()){};
    Server(const std::string &name, const size_t &port, const std::map<const int, const std::string> &errorPages,
           const size_t &clientMaxBodySize, const std::vector<const Location> &locations)
        : name(name), port(port), errorPages(errorPages), clientMaxBodySize(clientMaxBodySize), locations(locations){};
    Location getLocation(const std::string &target) const
    {
        unsigned int matchedIndex = 0;
        for (unsigned int i = 1; i < locations.size(); i++)
        {
            if (lengthOfPrefixMatch(target, locations[i].path) >
                lengthOfPrefixMatch(target, locations[matchedIndex].path))
                matchedIndex = i;
        }
        return locations[matchedIndex];
    };
};

typedef std::vector<const Server> Servers;
