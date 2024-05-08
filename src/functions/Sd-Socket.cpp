#include "../all.hpp"

Option<const Socket> findSocket(const int sd)
{
    for (Sockets::const_iterator it = SOCKETS.begin(); it != SOCKETS.end(); ++it)
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
    if (getsockname(sd, &addr, &addr_len) == 0)
    {
        return Option<const Socket>(Socket(sd, FILE_FD));
    }
    else if (errno == EINVAL)
    else
    {
        return Option<const Socket>();
    }
}
