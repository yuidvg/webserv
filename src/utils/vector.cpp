#include ".hpp"

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

FindSocketResult findSocket(const Sockets &sockets, const int descriptor)
{
    for (std::vector<Socket>::const_iterator it = sockets.begin(); it != sockets.end(); ++it)
    {
        if (it->descriptor == descriptor)
        {
            return FindSocketResult::Success(*it);
        }
    }
    return FindSocketResult::Error("socket not found");
}

} // namespace utils
