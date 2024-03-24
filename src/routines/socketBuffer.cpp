#include "../all.hpp"

void eraseSocketBuffer(const int sd)
{
    for (SocketBuffers::iterator it = SOCKET_BUFFERS.begin(); it != SOCKET_BUFFERS.end();)
    {
        if (it->sd == sd)
        {
            SOCKET_BUFFERS.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
}

bool appendOutbound(int sd, std::string content)
{
    FindSocketBufferResult findSocketBufferResult = findSocketBuffer(sd);
    if (findSocketBufferResult.success)
    {
        SocketBuffer &socketBuffer = findSocketBufferResult.value;
        socketBuffer.appendOutbound(content);
    }
    else
    {
        return false;
    }
}
