#include "../socket/.hpp"
#include "../all.hpp"
#include "../cgiRequest/.hpp"
#include "../cgiResponse/.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"

namespace
{
void handleListenSocketEvent(const Socket &listenSocket)
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

void handleInbound(SocketBuffer &socketIO)
{
    ConnectedSocketResult connectedSocketResult = getConnectedSocket(socketIO.sd);
    if (connectedSocketResult.success)
    {
        const ConnectedSocket &connectedSocket = connectedSocketResult.value;
        if (connectedSocket.tag == LEFT)
        {
            const ConnectedInternetSocket &clientSocket = connectedSocket.leftValue;
            handleClientMessages(clientSocket, socketIO);
        }
        else
        {
            const ConnectedUnixSocket &cgiSocket = connectedSocket.rightValue;
            
            handleCgiMessage(cgiSocket, socketIO.getInbound());
            socketIO.substringInbound(socketIO.getInbound().size());
        }
    }
    else
    {
        utils::printError(connectedSocketResult.error);
    }
}

void handleEvent(const struct kevent &event, const Sockets &listenSockets)
{
    const Sockets::const_iterator eventListenSocketIt = listenSockets.find(Socket(event.ident));
    if (eventListenSocketIt != listenSockets.end())
        handleListenSocketEvent(*eventListenSocketIt);
    else
    {
        for (SocketBuffers::iterator eventSocketIOIt = SOCKET_BUFFERS.begin(); eventSocketIOIt != SOCKET_BUFFERS.end();
             ++eventSocketIOIt)
        {
            if (eventSocketIOIt->sd == event.ident)
            {
                if (event.filter == EVFILT_READ)
                {
                    ReceiveResult receiveResult = eventSocketIOIt->receiveInbound(event.data);
                    if (receiveResult.success)
                    {
                        handleInbound(*eventSocketIOIt);
                    }
                    else
                        utils::printError(receiveResult.error);
                }
                else // EVFILT_WRITE
                {
                    SendResult sendResult = eventSocketIOIt->sendOutbound(event.data);
                    if (!sendResult.success)
                        utils::printError(sendResult.error);
                }
            }
        }
    }
}
} // namespace

void eventLoop(Sockets listenSockets)
{
    struct kevent eventList[EVENT_BATCH_SIZE];

    while (true)
    {
        std::cout << "waiting for events..." << std::endl;
        const int numOfEvents = kevent(KQ, NULL, 0, eventList, EVENT_BATCH_SIZE, NULL);
        if (numOfEvents != -1)
        {
            for (int i = 0; i < numOfEvents; ++i)
            {
                handleEvent(eventList[i], listenSockets);
            }
        }
    }
}
