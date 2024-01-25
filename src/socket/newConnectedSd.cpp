#include ".hpp"

NewListenSocketResult newConnectedSocket(const Socket listenSocket)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    const int newSd = accept(static_cast<int>(listenSocket.descriptor), (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (newSd < 0)
    {
        return NewListenSocketResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
    std::cout << "New conected socket: " << newSd << std::endl;
    return NewListenSocketResult::Success(Socket(newSd, listenSocket.port, ntohs(clientAddr.sin_addr), ntohs(clientAddr.sin_port)));
}
