#include "all.hpp"

namespace utils
{
IsDirectoryResult isDirectory(const std::string path)
{
    struct stat statbuf;
    if (stat(utils::toChar(path), &statbuf) != 0)
    {
        return IsDirectoryResult::Error(BAD_REQUEST_RESPONSE);
    }
    return IsDirectoryResult::Success(S_ISDIR(statbuf.st_mode));
}
} // namespace utils
