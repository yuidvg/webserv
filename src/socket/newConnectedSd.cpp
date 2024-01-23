#include ".hpp"

NewListenSdResult newConnectedSd(const Sd listenSd)
{
    const int newSd = accept(static_cast<int>(listenSd), NULL, NULL);
    if (newSd < 0)
    {
        return NewListenSdResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
    std::cout << "New conected socket: " << newSd << std::endl;
    return NewListenSdResult::Success(newSd);
}
