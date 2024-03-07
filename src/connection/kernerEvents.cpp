#include ".hpp"
namespace
{
bool registerEvent(const uintptr_t identifier, const int16_t filter)
{
    struct kevent change;
    EV_SET(&change, identifier, filter, EV_ADD, 0, 0, NULL);
    int numOfPlacedEvents = kevent(KQ, &change, 1, NULL, 0, NULL);
    if (numOfPlacedEvents != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
} // namespace

bool registerReadEvents(const Sockets &sockets)
{
    for (Sockets::const_iterator socketIt = sockets.begin(); socketIt != sockets.end(); ++socketIt)
    {
        if (!registerEvent(socketIt->descriptor, EVFILT_READ))
        {
            return false;
        }
    }
    return true;
}
