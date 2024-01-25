#include ".hpp"

namespace utils
{

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

std::string root(const std::string &uri, const Location &location)
{
    return location.root + uri.substr(location.path.length());
}
std::string concatPath(const std::string &pathA, const std::string &pathB)
{
    return pathA + (pathA[pathA.length() - 1] == '/' ? "" : "/") + pathB;
}

std::string index(const std::string &path, const Location &location)
{
    return concatPath(path, location.index);
}

} // namespace utils
