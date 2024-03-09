#include ".hpp"

bool updateEvent(const struct kevent &kevent, const short filter)
{
    if (!registerEvent(kevent.ident, kevent.filter, EV_DELETE))
        return false;
    if (!registerEvent(kevent.ident, filter, EV_ADD))
        return false;
    return true;
}

bool registerEvent(const uintptr_t identifier, const short filter, const short flags)
{
    struct kevent change;
    EV_SET(&change, identifier, filter, flags, 0, 0, NULL);
    int numOfPlacedEvents = kevent(KQ, &change, 1, NULL, 0, NULL);
    if (numOfPlacedEvents == -1)
    {
        std::cerr << std::strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool registerReadEvents(const Sockets &sockets)
{
    for (Sockets::const_iterator socketIt = sockets.begin(); socketIt != sockets.end(); ++socketIt)
    {
        if (!registerEvent(socketIt->descriptor, EVFILT_READ, EV_ADD))
        {
            return false;
        }
    }
    return true;
}
