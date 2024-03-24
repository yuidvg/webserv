#include "../all.hpp"

namespace utils
{

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
