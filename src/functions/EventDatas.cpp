#include "../all.hpp"

EventDatas retrieveDatas(const Events &clientReadEvents)
{
    EventDatas eventDatas;
    for (Events::const_iterator it = clientReadEvents.begin(); it != clientReadEvents.end(); ++it)
    {
        const Event &clientReadEvent = *it;
        char *buffer = new char[clientReadEvent.size + 1]; // +1 for null terminator
        const ssize_t receivedLength = recv(clientReadEvent.socket.descriptor, buffer, clientReadEvent.size, 0);
        if (receivedLength > 0)
        {
            buffer[receivedLength] = '\0';
            const std::string data(buffer);
            eventDatas.push_back(EventData(clientReadEvent.socket, data));
        }
        else if (receivedLength == 0)
        {
            close(clientReadEvent.socket.descriptor);
        }
        else
        {
            close(clientReadEvent.socket.descriptor);
        }
        delete[] buffer;
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

// clang-format off
EventDatas unifyData(EventDatas eventDatas)
{
    std::map<Socket, std::pair<std::string, HttpRequest> >unifiedEventDatas;
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        unifiedEventDatas[(*it).socket].first += (*it).data;
        unifiedEventDatas[(*it).socket].second = (*it).httpRequest;
    }
    EventDatas result;
    for (std::map<Socket, std::pair<std::string, HttpRequest> >::const_iterator it = unifiedEventDatas.begin();
         it != unifiedEventDatas.end(); ++it)
    {
        result.push_back(EventData(it->first, it->second.first, it->second.second));
    }
    return result;
}
// clang-format on

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
