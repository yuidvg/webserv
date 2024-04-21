#include "../all.hpp"

namespace utils
{
bool registerEvent(const uintptr_t identifier, const int16_t filter)
{
    struct kevent change;
    EV_SET(&change, identifier, filter, EV_ADD | (filter == EVFILT_READ ? EV_CLEAR : 0), 0, 0, NULL);
    int numOfPlacedEvents = kevent(KQ, &change, 1, NULL, 0, NULL);
    if (numOfPlacedEvents == -1)
    {
        std::cerr << std::strerror(errno) << std::endl;
        return false;
    }
    return true;
}
bool setEventFlags(const uintptr_t identifier, const int16_t filter, const uint16_t flags)
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

bool unregisterEvent(const uintptr_t identifier)
{
    struct kevent change;
    EV_SET(&change, identifier, 0, EV_DELETE, 0, 0, NULL);
    int numOfPlacedEvents = kevent(KQ, &change, 1, NULL, 0, NULL);
    if (numOfPlacedEvents == -1)
    {
        std::cerr << "unregisterEvent failed: " << std::strerror(errno) << std::endl;
        return false;
    }
    return true;
}

} // namespace utils
