#include "all.hpp"

unsigned int getMaxSd(const Sockets listenSockets)
{
    unsigned int maxSd = 0;
    for (size_t i = 0; i < listenSockets.size(); ++i)
    {
        if (listenSockets[i].descriptor > static_cast<int>(maxSd))
        {
            maxSd = listenSockets[i].descriptor;
        }
    }
    return maxSd;
}

ReadableSocketsResult readableSockets(const Sockets sockets)
{
    fd_set readableSocketSet = utils::fdSetFrom(sockets);
    std::cout << "Waiting for select()..." << std::endl;
    const int numOfReadableSDs = select(getMaxSd(sockets) + 1, &readableSocketSet, NULL, NULL, NULL);
    if (numOfReadableSDs < 0)
        return ReadableSocketsResult::Error("select() failed: " + std::string(strerror(errno)));
    else if (numOfReadableSDs == 0)
        return ReadableSocketsResult::Error("select() timed out. End program.");
    return ReadableSocketsResult::Success(utils::socketsIn(readableSocketSet, sockets));
}
