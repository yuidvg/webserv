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

    while (true)
    {
        try
        {
            std::cout << "waiting for events..." << std::endl;
            const int numOfEvents = kevent(KQ, NULL, 0, eventList, EVENT_BATCH_SIZE, NULL);
            if (numOfEvents != -1)
            {
                const KernelEvents kernelEvents = KernelEvents(eventList, eventList + numOfEvents);
                const Events events = toEvents(kernelEvents, sockets);
                // READ
                const Events readEvents = utils::filter(events, isReadEvent);
                //  INITIATE
                const Events initiateEvents = utils::filter(readEvents, isInitiateEvent);
                const Sockets newClientSockets = utils::newClientSockets(initiateEvents);
                sockets.insert(newClientSockets.begin(), newClientSockets.end());
                //  CLIENT
                const Events clientReadEvents = utils::filter(readEvents, isClientEvent);
                const EventDatas httpRequestDatas = retrieveDatas(clientReadEvents);
                //  CGI
                const Events cgiReadEvents = utils::filter(readEvents, isCgiEvent);
                // WRITE
                const Events writeEvents = utils::filter(events, isWriteEvent);
                //  CLIENT
                const Events clientWriteEvents = utils::filter(writeEvents, isClientEvent);
                //  CGI
                const Events cgiWriteEvents = utils::filter(writeEvents, isCgiEvent);
                //  FILE
                const Events fileWriteEvents = utils::filter(writeEvents, isFileEvent);
                const EventDatas cgiResponseDatas = utils::filter(eventDatas_errors.first, isCgiResponseData);
                const HttpRequestsAndEventDatas parsedHttpRequests = parseHttpRequests(httpRequestDatas);
                const CgiResponses cgiResponses = parseCgiResponses(cgiResponseDatas);
                const KernelEvents writeEvents = utils::filter(kernelEvents, isWriteEvent);
                for (int i = 0; i < numOfEvents; ++i)
                {
                    handleEvent(eventList[i], sockets);
                }
            }
            processHttpRequests();
        }
        catch (std::exception &e)
        {
            std::cerr << "eventLoop: " << e.what() << std::endl;
        }
    }
}
