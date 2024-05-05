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

bool isFileEvent(const Event &event)
{
    return event.socket.type == FILE_FD;
}

} // namespace

void eventLoop(Sockets sockets)
{
    struct kevent eventList[EVENT_BATCH_SIZE];
    EventDatas DANGLINGS;
    EventDatas OUTBOUNDS;

    while (true)
    {
        try
        {
            std::cout << "waiting for events..." << std::endl;
            const int numOfEvents = kevent(KQ, NULL, 0, eventList, EVENT_BATCH_SIZE, NULL);
            if (numOfEvents != -1)
            {
                const Events events = toEvents(KernelEvents(eventList, eventList + numOfEvents), sockets);
                // READ
                const Events readEvents = utils::filter(events, isReadEvent);
                //  CGI
                const Events cgiReadEvents = utils::filter(readEvents, isCgiEvent);
                const EventDatas cgiResponseDatas = retrieveDatas(cgiReadEvents);
                const CgiResponses cgiResponses = parseCgiResponses(cgiResponseDatas);
                const std::pair<const HttpResponses, const HttpRequests> httpResponses_httpRequests =
                    processCgiResponses(cgiResponses);
                const HttpResponses httpResponses = httpResponses_httpRequests.first;
                const HttpRequests localRedirectHttpRequests = httpResponses_httpRequests.second;
                const EventDatas httpResponseDatas = toEventDatas(httpResponses);
                utils::appendVector(OUTBOUNDS, httpResponseDatas);
                //  INITIATE
                const Events initiateEvents = utils::filter(readEvents, isInitiateEvent);
                const Sockets newClientSockets = utils::newClientSockets(initiateEvents);
                sockets.insert(newClientSockets.begin(), newClientSockets.end());
                //  CLIENT
                const Events clientReadEvents = utils::filter(readEvents, isClientEvent);
                const EventDatas httpRequestDatas = retrieveDatas(clientReadEvents);
                const std::pair<const HttpRequests, const EventDatas> httpRequests_danglings =
                    parseHttpRequests(utils::concat(DANGLINGS, httpRequestDatas));
                const HttpRequests httpRequests = httpRequests_danglings.first;
                DANGLINGS.clear();
                utils::appendVector(DANGLINGS, httpRequests_danglings.second);
                const std::pair<const HttpResponses, const CgiRequests> httpResponses_cgiRequests =
                    processHttpRequests(utils::concat(httpRequests, localRedirectHttpRequests));
                const EventDatas httpResponseEventDatas = toEventDatas(httpResponses_cgiRequests.first);
                const EventDatas cgiRequestEventDatas = toEventDatas(httpResponses_cgiRequests.second);
                utils::appendVector(OUTBOUNDS, httpResponseEventDatas);
                utils::appendVector(OUTBOUNDS, cgiRequestEventDatas);
                // WRITE
                const Events writeEvents = utils::filter(events, isWriteEvent);
                //  CLIENT
                const Events clientWriteEvents = utils::filter(writeEvents, isClientEvent);
                //  CGI
                const Events cgiWriteEvents = utils::filter(writeEvents, isCgiEvent);
                //  FILE
                const Events fileWriteEvents = utils::filter(writeEvents, isFileEvent);
            }
        }
        catch (std::exception &e)
        {
        }
    }
}
