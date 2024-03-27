#include "../all.hpp"

SocketBuffer &getSocketBuffer(const int sd)
{
    for (SocketBuffers::iterator it = SOCKET_BUFFERS.begin(); it != SOCKET_BUFFERS.end(); ++it)
    {
        if (it->sd == sd)
            return (*it);
    }
    SOCKET_BUFFERS.push_back(SocketBuffer(sd));
    return SOCKET_BUFFERS.back();
}
