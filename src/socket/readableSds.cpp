#include ".hpp"

ReadableSocketsResult readableSockets(const Sockets sds)
{
    fd_set readableSdSet = utils::fdSetFrom(sds);
    std::cout << "Waiting for select()..." << std::endl;
    const int numOfReadableSockets = select(utils::max(sds) + 1, &readableSdSet, NULL, NULL, NULL);
    if (numOfReadableSockets < 0)
        return ReadableSocketsResult::Error("select() failed: " + std::string(strerror(errno)));
    else if (numOfReadableSockets == 0)
        return ReadableSocketsResult::Error("select() timed out. End program.");
    return ReadableSocketsResult::Success(utils::sdsIn(readableSdSet, sds));
}
