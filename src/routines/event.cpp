#include "../all.hpp"

namespace
{
void createClientSocket(const Socket &listenSocket)
{
    const ConnectedInternetSocketResult newConnectedSocketResult = utils::newConnectedInternetSocket(listenSocket);
    if (newConnectedSocketResult.success)
    {
        CLIENT_SOCKETS.push_back(newConnectedSocketResult.value);
    }
    else
    {
        utils::printError(newConnectedSocketResult.error);
    }
}

ConnectedSocketResult getConnectedSocket(const int sd)
{
    for (ConnectedInternetSockets::iterator it = CLIENT_SOCKETS.begin(); it != CLIENT_SOCKETS.end(); ++it)
    {
        if (it->descriptor == sd)
            return ConnectedSocketResult::Success(ConnectedSocket::Left(*it));
    }
    for (ConnectedUnixSockets::iterator it = CGI_SOCKETS.begin(); it != CGI_SOCKETS.end(); ++it)
    {
        if (it->descriptor == sd)
            return ConnectedSocketResult::Success(ConnectedSocket::Right(*it));
    }
    return ConnectedSocketResult::Error("Failed to find connected socket with descriptor " + std::to_string(sd));
}

void handleInbound(SocketBuffer &socketBuffer)
{
    ConnectedSocketResult connectedSocketResult = getConnectedSocket(socketBuffer.sd);
    if (connectedSocketResult.success)
    {
        const ConnectedSocket &connectedSocket = connectedSocketResult.value;
        if (connectedSocket.tag == LEFT)
        {
            const ConnectedInternetSocket &clientSocket = connectedSocket.leftValue;
            processClientMessages(clientSocket, socketBuffer);
        }
        else
        {
            const ConnectedUnixSocket &cgiSocket = connectedSocket.rightValue;
            const std::string message = socketBuffer.getInbound();
            wipeCgi(cgiSocket.opponentPid, cgiSocket.descriptor);
            processCgiMessage(cgiSocket, message);
        }
    }
    else
    {
        utils::printError(connectedSocketResult.error);
    }
}

void handleEvent(const struct kevent &event, const Sockets &listenSockets)
{
    const Socket &eventListenSocket = *std::find(listenSockets.begin(), listenSockets.end(), event.ident);
    if (eventListenSocket.descriptor == static_cast<int>(event.ident))
        createClientSocket(eventListenSocket);
    else
    {
        SocketBuffer &eventSocketBuffer = getSocketBuffer(event.ident);
        if (eventSocketBuffer.sd == static_cast<int>(event.ident))
        {
            if (event.filter == EVFILT_READ)
            {
                ReceiveResult receiveResult = eventSocketBuffer.receiveInbound(event.data);
                if (receiveResult.success)
                {
                    handleInbound(eventSocketBuffer);
                }
                else
                    utils::printError(receiveResult.error);
            }
            else // EVFILT_WRITE
            {
                SendResult sendResult = eventSocketBuffer.sendOutbound(event.data);
                if (!sendResult.success)
                {
                    eraseSocketBuffer(eventSocketBuffer.sd);
                    utils::printError(sendResult.error);
                }
            }
        }
    }
}

bool isReadEvent(const struct kevent &event)
{
    return event.filter == EVFILT_READ;
}

bool isWriteEvent(const struct kevent &event)
{
    return event.filter == EVFILT_WRITE;
}

bool isHttpRequestData(const EventData &eventData)
{
    return std::find(CLIENT_SOCKETS.begin(), CLIENT_SOCKETS.end(), eventData.sd) != CLIENT_SOCKETS.end();
}

bool isCgiResponseData(const EventData &eventData)
{
    return std::find(CGI_SOCKETS.begin(), CGI_SOCKETS.end(), eventData.sd) != CGI_SOCKETS.end();
}

} // namespace

void eventLoop(Sockets listenSockets)
{
    struct kevent eventList[EVENT_BATCH_SIZE];

    while (true)
    {
        try
        {
            std::cout << "waiting for events..." << std::endl;
            const int numOfEvents = kevent(KQ, NULL, 0, eventList, EVENT_BATCH_SIZE, NULL);
            const KEvents events = KEvents(eventList, eventList + numOfEvents);
            if (numOfEvents != -1)
            {
                const KEvents readEvents = utils::filter(events, isReadEvent);
                const EventDatas eventDatas = retrieveDatas(readEvents);
                const EventDatas httpRequestDatas = utils::filter(eventDatas, isHttpRequestData);
                const EventDatas cgiResponseDatas = utils::filter(eventDatas, isCgiResponseData);
                const HttpRequestsAndEventDatas parsedHttpRequests = parseHttpRequests(httpRequestDatas);
                const CgiResponses cgiResponses = parseCgiResponses(cgiResponseDatas);
                const KEvents writeEvents = utils::filter(events, isWriteEvent);
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
