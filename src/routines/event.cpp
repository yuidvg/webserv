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

} // namespace
void eventLoop()
{
    struct kevent eventList[EVENT_BATCH_SIZE];
    EventDatas DANGLINGS;
    Outbounds OUTBOUNDS;

    while (true)
    {
        try
        {
            std::cout << "waiting for events..." << std::endl;
            // Set Write KEvents Availability
            const int numOfEvents = kevent(KQ, NULL, 0, eventList, EVENT_BATCH_SIZE, NULL);
            if (numOfEvents != -1)
            {
                const Events events = toEvents(KernelEvents(eventList, eventList + numOfEvents));
                // WRITE
                const Events writeEvents = utils::filter(events, isWriteEvent);
                OUTBOUNDS.dispatchEvents(writeEvents);
                // READ
                const Events readEvents = utils::filter(events, isReadEvent);
                //  CGI
                const Events cgiReadEvents = utils::filter(readEvents, isCgiEvent);
                const EventDatas cgiResponseDatas = retrieveDatas(cgiReadEvents, OUTBOUNDS);
                downCgis(cgiResponseDatas);
                const CgiResponses cgiResponses = parseCgiResponses(cgiResponseDatas);
                const std::pair< const HttpResponses, const HttpRequests > httpResponses_httpRequests =
                    processCgiResponses(cgiResponses);
                const HttpResponses httpResponsesFromCgi = httpResponses_httpRequests.first;
                const HttpRequests localRedirectHttpRequests = httpResponses_httpRequests.second;
                OUTBOUNDS.append_back(toEventDatas(httpResponsesFromCgi));
                //  INITIATE
                const Events initiateEvents = utils::filter(readEvents, isInitiateEvent);
                const Sockets newClientSockets = utils::newClientSockets(initiateEvents);
                SOCKETS.insert(newClientSockets.begin(), newClientSockets.end());
                //  CLIENT
                // Parse HttpRequests
                const Events clientReadEvents = utils::filter(readEvents, isClientEvent);
                const EventDatas httpRequestDatas = retrieveDatas(clientReadEvents, OUTBOUNDS);
                const std::pair< const HttpRequests, const EventDatas > httpRequests_danglings =
                    parseHttpRequests(unifyData(utils::concat(DANGLINGS, httpRequestDatas)));
                const HttpRequests httpRequests = httpRequests_danglings.first;
                DANGLINGS.clear();
                utils::appendVector(DANGLINGS, httpRequests_danglings.second);
                // Process HttpRequests
                const std::pair< const HttpResponses, const EventDatas > httpResponses_writeToFileEventDatas =
                    processHttpRequests(utils::concat(httpRequests, localRedirectHttpRequests));
                const HttpResponses httpResponses = httpResponses_writeToFileEventDatas.first;
                const EventDatas writeToFileEventDatas = httpResponses_writeToFileEventDatas.second;
                OUTBOUNDS.append_back(toEventDatas(httpResponses));
                OUTBOUNDS.append_back(writeToFileEventDatas);
            }
        }
        catch (std::exception &e)
        {
        }
    }
}
