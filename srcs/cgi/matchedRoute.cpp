#include "../config/config.hpp"

unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern)
{
    unsigned int i = 0;
    if (string.find(pattern) == 0)
        return pattern.length();
    else
        return 0;
}

Location matchedLocation(const std::string uri, const std::vector<Location> locations)
{
    Location matchedLocation = locations[0];
    for (unsigned int i = 1; i < locations.size(); i++)
    {
        Location location = locations[i];
        if (lengthOfPrefixMatch(uri, location.path) > lengthOfPrefixMatch(uri, matchedLocation.path))
            matchedLocation = location;
    }
    return matchedLocation;
}
