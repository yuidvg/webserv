#include ".hpp"

namespace utils
{
bool registerEvent(const uintptr_t identifier, const int16_t filter)
{
    struct kevent change;
    EV_SET(&change, identifier, filter, EV_ADD | (filter == EVFILT_WRITE ? EV_CLEAR : 0), 0, 0, NULL);
    int numOfPlacedEvents = kevent(KQ, &change, 1, NULL, 0, NULL);
    if (numOfPlacedEvents == -1)
    {
        std::cerr << std::strerror(errno) << std::endl;
        return false;
    }
    return true;
}
} // namespace utils
