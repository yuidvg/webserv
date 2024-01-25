#include ".hpp"

NewListenSocketResult newConnectedSocket(const Socket listenSocket)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    const int newSocket = accept(static_cast<int>(listenSocket), (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (newSocket < 0)
    {
        return NewListenSocketResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
    std::cout << "New conected socket: " << newSocket << std::endl;
    return NewListenSocketResult::Success(newSocket);
}
