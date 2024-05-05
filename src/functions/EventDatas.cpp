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


EventDatas