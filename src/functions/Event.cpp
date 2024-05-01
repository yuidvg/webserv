#include "../all.hpp"

namespace
{
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
} // namespace

Events toEvents(const KernelEvents &kernelEvents, const Sockets &sockets)
{
    Events events;
    for (KernelEvents::const_iterator it = kernelEvents.begin(); it != kernelEvents.end(); ++it)
    {
        const Option<Event> event = toEvent(*it, sockets);
        if (event)
        {
            events.push_back(*event);
        }
    }
    return events;
}
