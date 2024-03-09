#include ".hpp"

namespace utils
{
fd_set fdSetFrom(const Sockets sockets)
{
    fd_set fdSet;
    FD_ZERO(&fdSet);
    for (size_t i = 0; i < sockets.size(); ++i)
    {
        FD_SET(sockets[i].descriptor, &fdSet);
    }
    return fdSet;
}

Sockets socketsIn(const fd_set fdSet, const Sockets sockets)
{
    Sockets socketsInFdSet;
    for (size_t i = 0; i < sockets.size(); ++i)
    {
        if (FD_ISSET(sockets[i].descriptor, &fdSet))
        {
            socketsInFdSet.push_back(sockets[i]);
        }
    }
    return socketsInFdSet;
}

} // namespace utils