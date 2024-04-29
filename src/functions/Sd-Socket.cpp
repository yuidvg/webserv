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
    return Option<const Socket>();
}
