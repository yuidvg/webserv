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
    close(sd);
    return Option<const Socket>();
}
