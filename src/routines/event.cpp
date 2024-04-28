#include "../all.hpp"

namespace
{

bool isReadEvent(const struct kevent &event)
{
    return event.filter == EVFILT_READ;
}

bool isWriteEvent(const struct kevent &event)
{
    return event.filter == EVFILT_WRITE;
}

bool isClientEvent(std::pair<const int, const Sockets &> sd_clientSockets)
{
    const int sd = sd_clientSockets.first;
    const Sockets &clientSockets = sd_clientSockets.second;
    return std::find(clientSockets.begin(), clientSockets.end(), sd) != clientSockets.end();
}

bool isCgiEvent(const int sd, const Sockets &cgiSockets)
{
}

bool isInitiateEvent(const struct kevent &event, const Sockets &listenSockets)
{
    return std::find(listenSockets.begin(), listenSockets.end(), event.ident) != listenSockets.end();
}

} // namespace

void eventLoop(Sockets listenSockets)
{
    struct kevent eventList[EVENT_BATCH_SIZE];
    Sockets clientSockets;
    Sockets cgiSockets;

    while (true)
    {
        try
        {
            std::cout << "waiting for events..." << std::endl;
            const int numOfEvents = kevent(KQ, NULL, 0, eventList, EVENT_BATCH_SIZE, NULL);
            const std::pair<const Events, const std::string> 
            if (numOfEvents != -1)
            {
                const KernelEvents readEvents = utils::filter(events, isReadEvent);
                const KernelEvents initiateEvents = utils::filter(events, isInitiateEvent);
                const KernelEvents clientEvents = utils::filter(events, isClientEvent);
                const KernelEvents cgiEvents = utils::filter(events, isCgiEvent);
                const KernelEvents writeEvents = utils::filter(events, isWriteEvent);
                const SocketResult newConnectedSocketResult =
                    utils::newClientSocket(*listenSockets.find(eventList[0].ident));
                const std::pair<EventDatas, Strings> eventDatas_errors = retrieveDatas(readEvents);
                const EventDatas httpRequestDatas = utils::filter(eventDatas_errors.first, isHttpRequestData);
                const EventDatas cgiResponseDatas = utils::filter(eventDatas_errors.first, isCgiResponseData);
                const HttpRequestsAndEventDatas parsedHttpRequests = parseHttpRequests(httpRequestDatas);
                const CgiResponses cgiResponses = parseCgiResponses(cgiResponseDatas);
                const KernelEvents writeEvents = utils::filter(events, isWriteEvent);
                for (int i = 0; i < numOfEvents; ++i)
                {
                    handleEvent(eventList[i], listenSockets);
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
