#include ".hpp"

namespace utils {
bool registerEvent(const uintptr_t identifier, const int16_t filter)
{
    struct kevent writeEvent;
    EV_SET(&writeEvent, identifier, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    if (kevent(KQ, &writeEvent, 1, NULL, 0, NULL) != -1) {
        return true;
    } else {
        return false;
    }
}

} // namespace utils
