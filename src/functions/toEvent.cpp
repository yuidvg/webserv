#include "../all.hpp"

toEvent::toEvent(const Socket &socket) : socket(socket)
{
}

toEvent::toEvent(const toEvent &other) : sockets(other.sockets)
{
}

toEvent &toEvent::operator=(const toEvent &other)
{
    if (this != &other)
    {
        const_cast<Sockets &>(this->sockets) = other.sockets;
    }
    return *this;
}

toEvent::~toEvent()
{
}

toEvent::toEvent()
{
}

Option<Event> toEvent::operator()(const struct kevent &kernelEvent) const
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
