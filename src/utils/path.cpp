#include "../all.hpp"

namespace utils
{
std::string concatPath(const std::string &pathA, const std::string &pathB)
{
    return pathA + (pathA[pathA.length() - 1] != '/' && pathA.length() && pathB.length() ? "/" : "") + pathB;
}
std::string root(const std::string &path, const Location &location)
{
    return concatPath(location.root, location.path.length() <= path.size() ? path.substr(location.path.length()) : "");
}

std::string index(const std::string &path, const Location &location)
{
    if (utils::isDirectory(path))
        return concatPath(path, location.index);
    else
        return path;
}
std::string resolvePath(const std::string &path, const Location &location)
{
    return root(index(path, location), location);
}
} // namespace utils
