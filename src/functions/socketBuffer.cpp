#include "../all.hpp"

FindSocketBufferResult findSocketBuffer(const int sd)
{
    for (SocketBuffers::iterator it = SOCKET_BUFFERS.begin(); it != SOCKET_BUFFERS.end(); ++it)
    {
        if (it->sd == sd)
            return FindSocketBufferResult::Success(*it);
    }
    return FindSocketBufferResult::Error("Failed to find socket buffer with descriptor " + std::to_string(sd));
}


bool setOutbound(int sd, std::string content)
{
    FindSocketBufferResult findSocketBufferResult = findSocketBuffer(sd);
    if (findSocketBufferResult.success)
    {
        SocketBuffer &socketBuffer = findSocketBufferResult.value;
        socketBuffer.setOutbound(content);
    }
    else
    {
        return false;
    }
}
