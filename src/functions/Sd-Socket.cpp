#include "../all.hpp"

Option<const Socket> findSocket(const int sd, const Sockets &sockets)
{
    for (Sockets::const_iterator it = sockets.begin(); it != sockets.end(); ++it)
    {
        const Socket &socket = *it;
        if (socket.descriptor == sd)
        {
            return Option<const Socket>(socket);
        }
    }
    struct sockaddr addr;
    socklen_t addr_len = sizeof(addr);
    errno = 0;
    if (getsockname(sd, &addr, &addr_len) == -1 && errno == ENOTSOCK)
    {
        return Option<const Socket>(Socket(sd, FILE_FD));
    }
    else
    {
        return Option<const Socket>();
    }
}
