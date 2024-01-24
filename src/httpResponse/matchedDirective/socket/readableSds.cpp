#include ".hpp"

namespace
{
fd_set fdSetFrom(const Sds sds)
{
    fd_set fdSet;
    FD_ZERO(&fdSet);
    for (size_t i = 0; i < sds.size(); ++i)
    {
        FD_SET(sds[i], &fdSet);
    }
    return fdSet;
}

Sds sdsIn(const fd_set fdSet, const Sds sds)
{
    Sds sdsInFdSet;
    for (size_t i = 0; i < sds.size(); ++i)
    {
        if (FD_ISSET(sds[i], &fdSet))
        {
            sdsInFdSet.push_back(sds[i]);
        }
    }
    return sdsInFdSet;
}
} // namespace

ReadableSdsResult readableSds(const Sds sds)
{
    fd_set readableSdSet = fdSetFrom(sds);
    std::cout << "Waiting for select()..." << std::endl;
    const int numOfReadableSds = select(utils::max(sds) + 1, &readableSdSet, NULL, NULL, NULL);
    if (numOfReadableSds < 0)
        return ReadableSdsResult::Error("select() failed: " + std::string(strerror(errno)));
    else if (numOfReadableSds == 0)
        return ReadableSdsResult::Error("select() timed out. End program.");
    return ReadableSdsResult::Success(sdsIn(readableSdSet, sds));
}
