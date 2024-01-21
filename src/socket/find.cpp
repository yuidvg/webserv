#include "all.hpp"

ReadableSdsResult readableSds(const Sds sds)
{
    fd_set readableSdSet = utils::fdSetFrom(sds);
    std::cout << "Waiting for select()..." << std::endl;
    const int numOfReadableSds = select(utils::max(sds) + 1, &readableSdSet, NULL, NULL, NULL);
    if (numOfReadableSds < 0)
        return ReadableSdsResult::Error("select() failed: " + std::string(strerror(errno)));
    else if (numOfReadableSds == 0)
        return ReadableSdsResult::Error("select() timed out. End program.");
    return ReadableSdsResult::Success(utils::sdsIn(readableSdSet, sds));
}
