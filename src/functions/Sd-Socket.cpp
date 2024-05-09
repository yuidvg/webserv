#include "../all.hpp"

Option<const Socket> findSocket(const int sd)
{
    struct sockaddr addr;
    socklen_t addr_len = sizeof(addr);
    errno = 0;
    if (getsockname(sd, &addr, &addr_len) == 0) // socket
    {
        // unix domain socket
        if (addr.sa_family == AF_UNIX)
        {
            return Socket(sd, CGI);
        }
        else
        {
            struct sockaddr_in const *const sin = (struct sockaddr_in *)&addr;
            return Socket(sd, listen(sd, SOMAXCONN) == -1 ? CLIENT : INITIATE, ntohs(sin->sin_port));
        }
    }
    else if (errno == ENOTSOCK) // file
    {
        return Socket(sd, FILE_FD);
    }
    else
    {
        return Option<const Socket>();
    }
}
