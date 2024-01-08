#include "utils.hpp"

Location matchedLocation(const std::string uri, const std::vector<Location> routes)
{
    Location matchedLocation = routes[0];
    for (unsigned int i = 1; i < routes.size(); i++)
    {
        const Location route = routes[i];
        if (utils::lengthOfPrefixMatch(uri, route.path) > utils::lengthOfPrefixMatch(uri, matchedLocation.path))
            matchedLocation = route;
    }
    return matchedLocation;
}
