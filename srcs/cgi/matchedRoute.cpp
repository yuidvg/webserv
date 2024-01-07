#include "../config/config.hpp"

const unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern)
{
    if (string.find(pattern) == 0)
        return pattern.length();
    else
        return 0;
}

const Route matchedRoute(const std::string uri, const std::vector<Route> routes)
{
    Route matchedRoute = routes[0];
    for (unsigned int i = 1; i < routes.size(); i++)
    {
        const Route route = routes[i];
        if (lengthOfPrefixMatch(uri, route.path) > lengthOfPrefixMatch(uri, matchedRoute.path))
            matchedRoute = route;
    }
    return matchedRoute;
}
