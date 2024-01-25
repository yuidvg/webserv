#include ".hpp"

Location getLocation(const std::string &target, const std::vector<const Location> &locations)
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
