#include "../all.hpp"

Parsed parseDatas(const HTTP_REQUESTS &eventDatas)
{
    Parsed parsed;
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        const EventData &eventData = *it;

    }
    return parsed;
}