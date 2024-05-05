#include "../all.hpp"

namespace
{
EventDatas matchEventDatas(const EventDatas &eventDatas, const Event &event)
{
    EventDatas filteredEventDatas;
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        const EventData &eventData = *it;
        if (eventData.socket == event.socket)
        {
            filteredEventDatas.push_back(eventData);
        }
    }
    return filteredEventDatas;
}

EventDatas sendEventData(const intptr_t size, const EventDatas &eventDatas)
{
    EventDatas leftoverEventDatas;
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        const EventData &eventData = *it;
        if (eventData.data.size() > size)
        {
            const std::string leftoverData = eventData.data.substr(size);
            const EventData leftoverEventData(eventData.socket, leftoverData);
            leftoverEventDatas.push_back(leftoverEventData);
        }
    }
    return leftoverEventDatas;
}

EventDatas sendEventData(intptr_t size, const EventDatas &eventDatas)
{
    EventDatas leftoverEventDatas;
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        const EventData &eventData = *it;
        const intptr_t writtenSize = write(eventData.socket.descriptor, eventData.data.c_str(), eventData.data.size());
        if (writtenSize <= 0)
        {
            close(eventData.socket.descriptor);
        }
        else if (writtenSize < eventData.data.size())
        {
            const std::string leftoverData = eventData.data.substr(writtenSize);
            const EventData leftoverEventData(eventData.socket, leftoverData);
            leftoverEventDatas.push_back(leftoverEventData);
        }
    }
    return leftoverEventDatas;
}

} // namespace

EventDatas sendEventDatas(const Events &events, const EventDatas &eventDatas)
{
    for (Events::const_iterator it = events.begin(); it != events.end(); ++it)
    {
        const Event &event = *it;
        const EventDatas &matchedEventDatas = matchEventDatas(eventDatas, event);
        for (EventDatas::const_iterator it = matchedEventDatas.begin(); it != matchedEventDatas.end(); ++it)
        {
            const EventData &eventData = *it;
            const EventDatas leftoverEventDatas = sendEventData(event.size, matchedEventDatas);
        }
    }
}
