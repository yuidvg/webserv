#include "../all.hpp"

namespace
{

Option<EventData> sendEventData(intptr_t size, const EventData &eventData)
{

    const intptr_t writtenSize = send(eventData.socket.descriptor, eventData.data.c_str(),
                                      std::min(static_cast<size_t>(size), eventData.data.size()), 0);
    if (writtenSize > 0)
    {
        const std::string leftoverData = eventData.data.substr(writtenSize);
        const EventData leftoverEventData(eventData.socket, leftoverData);
        if (leftoverEventData.data.size() > 0)
        {
            return Option<EventData>(leftoverEventData);
        }
        else
        {
            close(eventData.socket.descriptor);
        }
    }
    else // writtenSize == 0 or writtenSize == -1
    {
        close(eventData.socket.descriptor);
        return Option<EventData>();
    }
}

} // namespace

EventDatas sendEventDatas(const Events &events, const EventDatas &unifiedDatas)
{
    EventDatas leftoverEventDatas;
    for (Events::const_iterator it = events.begin(); it != events.end(); ++it)
    {
        const Event &event = *it;
        const Option<EventData> &matchedEventData = findEventData(event.socket.descriptor, unifiedDatas);
        if (matchedEventData)
        {
            const Option<EventData> leftoverEventData = sendEventData(event.size, (*matchedEventData));
            if (leftoverEventData)
            {
                leftoverEventDatas.push_back(*leftoverEventData);
            }
        }
    }
    return leftoverEventDatas;
}
