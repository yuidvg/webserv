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

HttpResponse writeEventData(const EventData &eventData)
{
    const size_t writtenSize = write(eventData.socket.descriptor, eventData.data.c_str(), eventData.data.size());
    if (writtenSize == eventData.data.size())
    {
        return HttpResponse(eventData.socket.descriptor, SUCCESS, eventData.data, "text/plain");
    }
    else
    {
        close(eventData.socket.descriptor);
        return getErrorHttpResponse(HttpRequest(eventData.socket.descriptor), SERVER_ERROR);
    }
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

HttpResponses writeEventDatas(const EventDatas &eventDatas)
{
    HttpResponses httpResponses;
    for (EventDatas::const_iterator it = eventDatas.begin(); it != eventDatas.end(); ++it)
    {
        const EventData &eventData = *it;
        const HttpResponse httpResponse = writeEventData(eventData);
        httpResponses.push_back(httpResponse);
    }
    return httpResponses;
}