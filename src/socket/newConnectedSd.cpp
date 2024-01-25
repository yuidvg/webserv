#include ".hpp"

NewListenSocketResult newConnectedSd(const Socket listenSd)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    const int newSd = accept(static_cast<int>(listenSd), (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (newSd < 0)
    {
        return NewListenSocketResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
    std::cout << "New conected socket: " << newSd << std::endl;
    return NewListenSocketResult::Success(newSd);
}
