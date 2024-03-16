#include ".hpp"

NewConnectionResult newConnectedSocket(const Connection &listenSocket)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    const int newSd = accept(listenSocket.sd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (newSd >= 0)
    {
        if (utils::registerEvent(newSd, EVFILT_READ) && utils::registerEvent(newSd, EVFILT_WRITE))
        {
            return NewConnectionResult::Success(Socket(
                newSd, listenSocket.port, std::string(inet_ntoa(clientAddr.sin_addr)), ntohs(clientAddr.sin_port)));
        }
        else
        {
            return NewConnectionResult::Error("Failed to register new conected socket: " +
                                              std::string(strerror(errno)));
        }
    }
    else
    {
        return NewConnectionResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
}
