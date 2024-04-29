#include "../all.hpp"

Option<Event> toEvent(const struct kevent &kernelEvent, const Sockets &sockets)
{
    const Option<const Socket> socket = findSocket(kernelEvent.ident, sockets);
    if (socket)
    {
        return Option<Event>(Event(*socket, kernelEvent.filter == EVFILT_READ ? READ : WRITE, kernelEvent.data));
    }
    else
    {
        return Option<Event>();
    }
}
