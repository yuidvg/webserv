#include "../all.hpp"

namespace
{

bool isReadEvent(const Event &event)
{
    return event.type == READ;
}

bool isWriteEvent(const Event &event)
{
    return event.type == WRITE;
}

bool isClientEvent(const Event &event)
{
    return event.socket.type == CLIENT;
}

bool isCgiEvent(const Event &event)
{
    return event.socket.type == CGI;
}

bool isInitiateEvent(const Event &event)
{
    return event.socket.type == INITIATE;
}

bool isFileEventData(const EventData &eventData)
{
    return eventData.socket.type == FILE_FD;
}

bool isNotFileEventData(const EventData &eventData)
{
    return eventData.socket.type != FILE_FD;
}

EventDatas filterEventOutboundDatas(const Events &writeEvents, const EventDatas &unifiedOutbounds)
{
    EventDatas toBeWrittenDatas;
    for (Events::const_iterator it = writeEvents.begin(); it != writeEvents.end(); ++it)
    {
        const Event &event = *it;
        const Option<EventData> &matchedEventData = findEventData(event.socket.descriptor, unifiedOutbounds);
        if (matchedEventData)
        {
            toBeWrittenDatas.push_back(*matchedEventData);
        }
    }
    return toBeWrittenDatas;
}
} // namespace
void eventLoop()
{
    struct kevent eventList[EVENT_BATCH_SIZE];
    EventDatas DANGLINGS;

    while (true)
    {
        try
        {
            std::cout << "waiting for events..." << std::endl;
            // Set Write KEvents Availability
            for (EventDatas::const_iterator it = OUTBOUNDS.begin(); it != OUTBOUNDS.end(); ++it)
            {
                if (utils::setEventFlags((*it).socket.descriptor, EVFILT_WRITE, EV_ENABLE) == false)
                {
                    close((*it).socket.descriptor);
                    OUTBOUNDS.erase(it);
                }
            }
            const int numOfEvents = kevent(KQ, NULL, 0, eventList, EVENT_BATCH_SIZE, NULL);
            if (numOfEvents != -1)
            {
                const Events events = toEvents(KernelEvents(eventList, eventList + numOfEvents));
                // READ
                const Events readEvents = utils::filter(events, isReadEvent);
                //  CGI
                const Events cgiReadEvents = utils::filter(readEvents, isCgiEvent);
                const EventDatas cgiResponseDatas = retrieveDatas(cgiReadEvents);
                downCgis(cgiResponseDatas);
                const CgiResponses cgiResponses = parseCgiResponses(cgiResponseDatas);
                const std::pair<const HttpResponses, const HttpRequests> httpResponses_httpRequests =
                    processCgiResponses(cgiResponses);
                const HttpResponses httpResponsesFromCgi = httpResponses_httpRequests.first;
                const HttpRequests localRedirectHttpRequests = httpResponses_httpRequests.second;
                utils::appendVector(OUTBOUNDS, toEventDatas(httpResponsesFromCgi));
                //  INITIATE
                const Events initiateEvents = utils::filter(readEvents, isInitiateEvent);
                const Sockets newClientSockets = utils::newClientSockets(initiateEvents);
                SOCKETS.insert(newClientSockets.begin(), newClientSockets.end());
                //  CLIENT
                // Parse HttpRequests
                const Events clientReadEvents = utils::filter(readEvents, isClientEvent);
                const EventDatas httpRequestDatas = retrieveDatas(clientReadEvents);
                const std::pair<const HttpRequests, const EventDatas> httpRequests_danglings =
                    parseHttpRequests(unifyData(utils::concat(DANGLINGS, httpRequestDatas)));
                const HttpRequests httpRequests = httpRequests_danglings.first;
                DANGLINGS.clear();
                utils::appendVector(DANGLINGS, httpRequests_danglings.second);
                // Process HttpRequests
                const HttpResponses httpResponses =
                    processHttpRequests(utils::concat(httpRequests, localRedirectHttpRequests));
                // OUTBOUND HttpResponses
                const EventDatas &httpResponseEventDatas = toEventDatas(httpResponses);
                utils::appendVector(OUTBOUNDS, httpResponseEventDatas);
                // WRITE
                const Events writeEvents = utils::filter(events, isWriteEvent);
                const EventDatas eventOutboundDatas = filterEventOutboundDatas(writeEvents, OUTBOUNDS);
                utils::excludeVector(OUTBOUNDS, eventOutboundDatas);
                const EventDatas nonFileOutboundDatas = utils::filter(eventOutboundDatas, isNotFileEventData);
                const EventDatas leftoverEventDatas = sendEventDatas(nonFileOutboundDatas);
                const EventDatas fileOutboundDatas = utils::filter(eventOutboundDatas, isFileEventData);
                writeEventDatas(fileOutboundDatas);
                utils::appendVector(OUTBOUNDS, leftoverEventDatas);
            }
        }
        catch (std::exception &e)
        {
        }
    }
}
