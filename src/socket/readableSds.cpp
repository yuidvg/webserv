#include ".hpp"

namespace
{
Socket maxSdSocket(const Sockets &sockets)
{
    unsigned int maxIndex = 0;
    for (unsigned int i = 0; i < sockets.size(); i++)
        if (sockets[i].descriptor > sockets[maxIndex].descriptor)
            maxIndex = i;
    return sockets[maxIndex];
}
} // namespace

