#include "all.hpp"

namespace utils
{

Location matchedLocation(const std::string uri, const std::vector<Location> routes)
{
    unsigned int matchedIndex = 0;
    for (unsigned int i = 1; i < routes.size(); i++)
    {
        if (utils::lengthOfPrefixMatch(uri, routes[i].path) >
            utils::lengthOfPrefixMatch(uri, routes[matchedIndex].path))
            matchedIndex = i;
    }
    return routes[matchedIndex];
}

std::string root(const std::string uri, const Location location)
{
    return location.root + uri.substr(location.path.length());
}

std::string index(const std::string path, const Location location)
{
    return concatPath(path, location.index);
}

std::string concatPath(const std::string pathA, const std::string pathB)
{
    return pathA + (pathA[pathA.length() - 1] == '/' ? "" : "/") + pathB;
}
} // namespace utils
