#include "../all.hpp"

namespace
{
Option<EventData> retrieveData(const Event &readEvent)
{
    char *buffer = new char[readEvent.size + 1]; // +1 for null terminator
    const ssize_t receivedLength = recv(readEvent.socket.descriptor, buffer, readEvent.size, 0);
    if (receivedLength > 0)
    {
        buffer[receivedLength] = '\0';
        const std::string data(buffer);
        delete[] buffer;
        return EventData(readEvent.socket, data);
    }
    else
    {
        delete[] buffer;
        return Option<EventData>();
    }
}
} // namespace

EventDatas retrieveDatas(const Events &readEvents)
{
    EventDatas eventDatas;
    for (Events::const_iterator it = readEvents.begin(); it != readEvents.end(); ++it)
    {
        const Event &readEvent = *it;
        const Option<EventData> &eventData = retrieveData(readEvent);
        if (eventData)
        {
            eventDatas.push_back(*eventData);
        }
    }
    return eventDatas;
}

EventDatas removeDuplicates(EventDatas eventDatas)
{
    EventDatas uniqueEventDatas;
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        const EventData &eventData = *it;
        bool isUnique = true;
        for (EventDatas::const_iterator it2 = uniqueEventDatas.begin(); it2 != uniqueEventDatas.end(); ++it2)
        {
            const EventData &uniqueEventData = *it2;
            if (eventData.socket.descriptor == uniqueEventData.socket.descriptor)
            {
                isUnique = false;
                break;
            }
        }
        if (isUnique)
        {
            uniqueEventDatas.push_back(eventData);
        }
    }
    return uniqueEventDatas;
}

EventDatas unifyData(EventDatas eventDatas)
{
    std::map<Socket, std::string> unifiedEventDatas;
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        unifiedEventDatas[(*it).socket] += (*it).data;
    }
    EventDatas result;
    for (std::map<Socket, std::string>::const_iterator it = unifiedEventDatas.begin(); it != unifiedEventDatas.end();
         ++it)
    {
        result.push_back(EventData(it->first, it->second));
    }
    return result;
}

Option<EventData> findEventData(const int fd, const EventDatas &eventDatas)
{
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        const EventData &eventData = *it;
        if (eventData.socket.descriptor == fd)
        {
            return Option<EventData>(eventData);
        }
    }
    return Option<EventData>();
}
