#include ".hpp"

namespace
{
Socket maxSdSocket(const Sockets &sockets)
{
    unsigned int maxIndex = 0;
    for (unsigned int i = 0; i < sockets.size(); i++)
        if (sockets[i].descriptor > sockets[maxIndex].descriptor)
            maxIndex = i;
    return sockets[maxIndex];
}
} // namespace

ReadableSocketsResult readableSockets(const Sockets sockets)
{
    fd_set readableSocketSet = utils::fdSetFrom(sockets);
    std::cout << "Waiting for select()..." << std::endl;
    const int numOfReadableSockets = select(maxSdSocket(sockets).descriptor + 1, &readableSocketSet, NULL, NULL, NULL);
    if (numOfReadableSockets < 0)
        return ReadableSocketsResult::Error("select() failed: " + std::string(strerror(errno)));
    else if (numOfReadableSockets == 0)
        return ReadableSocketsResult::Error("select() timed out. End program.");
    return ReadableSocketsResult::Success(utils::sdsIn(readableSocketSet, sockets));
}
