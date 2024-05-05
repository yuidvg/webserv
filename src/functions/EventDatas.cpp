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

EventDatas unifyData(EventDatas eventDatas)
{
    EventDatas unifiedEventDatas;
    while (!eventDatas.empty())
    {
        EventDatas::iterator it = eventDatas.begin();
        const EventData &eventData = *it;
        EventDatas::iterator it2 = it;
        ++it2;
        while (it2 != eventDatas.end())
        {
            const EventData &eventData2 = *it2;
            if (eventData.socket == eventData2.socket)
            {
                const std::string unifiedData = eventData.data + eventData2.data;
                const EventData unifiedEventData(eventData.socket, unifiedData);
                if (unifiedEventData.data.size() > 0)
                {
                    unifiedEventDatas.push_back(unifiedEventData);
                }
                eventDatas.erase(it);
                eventDatas.erase(it2);
                break;
            }
            ++it2;
        }
    }
    return unifiedEventDatas;
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
