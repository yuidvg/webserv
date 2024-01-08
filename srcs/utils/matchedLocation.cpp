

const unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern)
{
    if (string.find(pattern) == 0)
        return pattern.length();
    else
        return 0;
}

const Route matchedLocation(const std::string uri, const std::vector<Route> routes)
{
    Route matchedLocation = routes[0];
    for (unsigned int i = 1; i < routes.size(); i++)
    {
        const Route route = routes[i];
        if (lengthOfPrefixMatch(uri, route.path) > lengthOfPrefixMatch(uri, matchedLocation.path))
            matchedLocation = route;
    }
    return matchedLocation;
}
