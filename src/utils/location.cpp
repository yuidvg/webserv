#include "all.hpp"

namespace utils
{

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

std::string rooted(const std::string uri, const Location location)
{
    return location.root + uri.substr(location.path.length());
}

std::string indexed(const std::string path, const Location location)
{
    return concatPath(path, location.index);
}

std::string concatPath(const std::string pathA, const std::string pathB)
{
    return pathA + (pathA[pathA.length() - 1] == '/' ? "" : "/") + pathB;
}
} // namespace utils
