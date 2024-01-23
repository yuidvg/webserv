#include "all.hpp"

namespace utils
{
std::string join(const std::vector<std::string> &list, const std::string &delimiter)
{
    std::string str;
    for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        if (it != list.begin())
            str += delimiter;
        str += *it;
    }
    return str;
}
} // namespace utils
