#include "../socket/.hpp"
#include "../cgiRequest/.hpp"
#include "../cgiResponse/.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"
#include ".hpp"

namespace
{
void handleListenSocketEvent(const Socket &listenSocket)
{
    const ConnectedInternetSocketResult newConnectedSocketResult = newConnectedInternetSocket(listenSocket);
    if (newConnectedSocketResult.success)
        CLIENTS += newConnectedSocketResult.value;
    else
        utils::printError(newConnectedSocketResult.error);
}

void handleMessageFromClient(Client &client, size_t messageSize)
{
    if (client.receiveMessage(messageSize))
    {
    }
    else
    {
        utils::printError("failed to receive message: invalid socket.");
        close(client.sd);
        CLIENTS -= client;
    }
}

void handleClientEvent(const struct kevent event, Client &client)
{
    if (!(event.flags & EV_ERROR))
    {
        if (event.filter == EVFILT_READ)
        {
            if (!(event.flags & EV_EOF))
                handleMessageFromClient(client, event.data);
            else
                std::cout << "socket " << client.sd << " is close\n" << std::endl;
        }
        else if (event.filter == EVFILT_WRITE)
        {
            if (!(event.flags & EV_EOF))
            {
                if (!client.sendMessage(event.data))
                {
                    std::cerr << "socket " << client.sd << " is broken\n" << std::endl;
                    close(client.sd);
                    CLIENTS -= client;
                }
            }
            else
            {
                std::cout << "client " << client.sd << " findEventObject disconnected" << std::endl;
                close(client.sd);
                CLIENTS -= client;
            }
        }
    }
    else
    {
        utils::printError("kernel event error");
    }
}

void handleCgiEvent(struct kevent event, Cgi &eventCgi)
{
    if (!(event.flags & EV_ERROR))
    {
        if (event.filter == EVFILT_READ)
        {
            if (!(event.flags & EV_EOF))
            {
                if (eventCgi.receiveMessage(event.data))
                {
                    std::cout << "received message from cgi " << eventCgi.sd << std::endl;
                }
                else
                {
                    std::cerr << "socket " << eventCgi.sd << " is broken\n" << std::endl;
                    close(eventCgi.sd);
                    CGIS -= eventCgi;
                }
            }
            else
            {
                close(eventCgi.sd);
                CGIS -= eventCgi;
                const ParseCgiResponseResult parseCgiResponseResult = parseCgiResponse(eventCgi.getReceivedMessage());
                if (parseCgiResponseResult.success)
                {
                    CgiResponse cgiResponse = parseCgiResponseResult.value;
                    processCgiResponse(
                        cgiResponse, eventCgi.httpRequest, *eventCgi.getClient(),
                        CONFIG.getServer(eventCgi.httpRequest.host, eventCgi.getClient()->serverPort).errorPages);
                }
                else
                {
                    eventCgi.getClient()->appendToBeSentMessage(responseText());
                }
            }
        }
    }
    else if (event.filter == EVFILT_WRITE)
    {
        if (!(event.flags & EV_EOF))
        {
            if (!eventCgi.sendMessage(event.data))
            {
                std::cerr << "socket " << eventCgi.sd << " is broken\n" << std::endl;
                close(eventCgi.sd);
                CGIS -= eventCgi;
            }
        }
        else
        {
            std::cout << "cgi " << eventCgi.sd << " findEventObject disconnected" << std::endl;
            close(eventCgi.sd);
            CGIS -= eventCgi;
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
                const struct kevent &event = eventList[i];
                const Sockets::const_iterator eventListenSocketIt = listenSockets.find(Socket(event.ident));
                if (eventListenSocketIt != listenSockets.end())
                    handleListenSocketEvent(*eventListenSocketIt);
                else
                {
                    const Clients::const_iterator eventClientIt = CLIENT_SDS.find(Client(event.ident));
                    if (eventClientIt != CLIENTS.end())
                        handleClientEvent(event, *eventClientIt);
                    else
                    {
                        const Cgis::const_iterator eventCgiIt = CGI_SDS.find(Cgi(event.ident));
                        if (eventCgiIt != CGIS.end())
                            handleCgiEvent(event, *eventCgiIt);
                        else
                            utils::printError("unexpected event");
                    }

                }
            }
        }
        else
        {
            utils::printError("unexpected event");
        }
    }
}
