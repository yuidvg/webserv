#include "../all.hpp"

namespace
{
EventData toEventData(const Event &event, const Sockets &sockets)
{
    return EventData(event, sockets);
}

} // namespace

EventDatas toEventDatas(const HttpResponses &httpResponses)
{
    EventDatas eventDatas;
    for (Events::const_iterator it = events.begin(); it != events.end(); ++it)
    {
        eventDatas.push_back(toEventData(*it, sockets));
    }
    return eventDatas;
}

EventDatas toEventDatas(const CgiResponses &cgiResponses)
{
    EventDatas eventDatas;
    for (CgiResponses::const_iterator it = cgiResponses.begin(); it != cgiResponses.end(); ++it)
    {
        eventDatas.push_back(toEventData(*it, sockets));
    }
    return eventDatas;
}
