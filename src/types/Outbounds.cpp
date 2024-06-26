#include "Outbounds.hpp"
#include "../all.hpp"

EventDatas::iterator Outbounds::outboundWithSameSocket(const EventData &eventData)
{
    for (EventDatas::iterator it = outbounds.begin(); it != outbounds.end(); ++it)
    {
        if (it->socket.descriptor == eventData.socket.descriptor)
        {
            return it;
        }
    }
    return outbounds.end();
}

EventDatas::iterator Outbounds::findOutbound(const Socket &socket)
{
    for (EventDatas::iterator it = outbounds.begin(); it != outbounds.end(); ++it)
    {
        if (it->socket.descriptor == socket.descriptor)
        {
            return it;
        }
    }
    return outbounds.end();
}

Outbounds::Outbounds() : outbounds()
{
}

Outbounds::~Outbounds()
{
}

void Outbounds::remove(const Socket &socket)
{
    EventDatas::iterator outboundIt = findOutbound(socket);
    if (outboundIt != outbounds.end())
    {
        utils::eraseRemove(outbounds, *outboundIt);
    }
}

void Outbounds::push_back(const EventData &eventData)
{
    if (eventData.data.empty())
    {
        return;
    }
    EventDatas::iterator outboundWithSameSocketIt = outboundWithSameSocket(eventData);
    if (outboundWithSameSocketIt != outbounds.end())
    {
        (*outboundWithSameSocketIt).data += eventData.data;
    }
    else // eventData with a new socket
    {
        if (utils::setEventFlags(eventData.socket.descriptor, EVFILT_WRITE, EV_ENABLE))
        {
            outbounds.push_back(eventData);
        }
        else
        {
            if (SOCKETS.find(eventData.socket) != SOCKETS.end())
            {
                SOCKETS.erase(SOCKETS.find(eventData.socket));
            }
            close(eventData.socket.descriptor);
        }
    }
}

void Outbounds::append_back(const EventDatas &eventDatas)
{
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        push_back(*it);
    }
}

void Outbounds::dispatchEvents(const Events &events)
{
    for (Events::const_iterator it = events.begin(); it != events.end(); ++it)
    {
        const EventDatas::iterator outboundIt = findOutbound(it->socket);
        if (outboundIt != outbounds.end())
        {
            const EventData &eventData = *outboundIt;
            const ssize_t writtenSize =
                write(eventData.socket.descriptor, eventData.data.c_str(), eventData.data.size());
            if (writtenSize > 0)
            {
                removeClient(eventData.socket, *this);
            }
            else // writtenSize == 0 or -1
            {
                removeClient(eventData.socket, *this);
            }
        }
        else
        {
            utils::setEventFlags(it->socket.descriptor, EVFILT_WRITE, EV_DISABLE);
            // removeClient(it->socket, *this);
        }
    }
}

void Outbounds::refresh()
{
    for (EventDatas::iterator it = outbounds.begin(); it != outbounds.end();)
    {
        if (SOCKETS.find(it->socket) == SOCKETS.end())
        {
            close(it->socket.descriptor);
            it = outbounds.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
