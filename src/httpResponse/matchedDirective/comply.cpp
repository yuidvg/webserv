#include ".hpp"

namespace
{
std::string root(const std::string &path, const Location &location)
{
    return location.root + path.substr(location.path.length());
}

std::string concatPath(const std::string &pathA, const std::string &pathB)
{
    return pathA + (pathA[pathA.length() - 1] == '/' ? "" : "/") + pathB;
}

std::string index(const std::string &path, const Location &location)
{
    return concatPath(path, location.index);
}

} // namespace
std::string comply(const std::string &path, const Location &location)
{
    return root(index(path, location), location);
}
