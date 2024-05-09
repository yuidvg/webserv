#include "../all.hpp"

namespace
{

Option<EventData> sendEventData(const EventData &eventData)
{

    const intptr_t writtenSize = send(eventData.socket.descriptor, eventData.data.c_str(), eventData.data.size(), 0);
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
            return Option<EventData>();
        }
    }
    else // writtenSize == 0 or writtenSize == -1
    {
        close(eventData.socket.descriptor);
        return Option<EventData>();
    }
}

void writeEventData(const EventData &eventData)
{
    write(eventData.socket.descriptor, eventData.data.c_str(), eventData.data.size());
    close(eventData.socket.descriptor);
}

} // namespace

EventDatas sendEventDatas(const EventDatas &eventDatas)
{
    EventDatas leftoverEventDatas;
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        const EventData &eventData = *it;
        Option<EventData> leftoverEventData = sendEventData(eventData);
        if (leftoverEventData)
            leftoverEventDatas.push_back(*leftoverEventData);
    }
    return leftoverEventDatas;
}

void writeEventDatas(const EventDatas &eventDatas)
{
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
        writeEventData(*it);
}
