#include ".hpp"

NewClientResult newConnection(const Socket &listenSocket)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    const int newSd = accept(listenSocket.descriptor, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (newSd >= 0)
    {
        if (utils::registerEvent(newSd, EVFILT_READ) && utils::registerEvent(newSd, EVFILT_WRITE))
        {
            return NewClientResult::Success(Client(
                newSd, listenSocket.port, std::string(inet_ntoa(clientAddr.sin_addr)), ntohs(clientAddr.sin_port)));
        }
        else
        {
            return NewClientResult::Error("Failed to register new conected socket: " + std::string(strerror(errno)));
        }
    }
    else
    {
        return NewClientResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
}
