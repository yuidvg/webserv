#include "all.hpp"

namespace utils
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

} // namespace utils