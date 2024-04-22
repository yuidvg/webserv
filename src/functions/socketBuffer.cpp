#include "../all.hpp"

SocketBuffer &getSocketBuffer(const int sd)
{
    for (SocketBuffers::iterator it = SOCKET_BUFFERS.begin(); it != SOCKET_BUFFERS.end(); ++it)
    {
        if (it->sd == sd)
            return (*it);
    }
    return *SOCKET_BUFFERS.begin();
}
