#include "all.hpp"

namespace utils
{
IsDirectoryResult isDirectory(const std::string path)
{
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
    {
        return IsDirectoryResult::Error(HttpResponse(BAD_REQUEST, Headers(), ""));
    }
    return IsDirectoryResult::Success(S_ISDIR(statbuf.st_mode));
}
} // namespace utils
