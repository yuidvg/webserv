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
    const NewClientResult newConnectedSocketResult = newConnection(listenSocket);
    if (newConnectedSocketResult.success)
        CLIENTS += newConnectedSocketResult.value;
    else
        utils::printError(newConnectedSocketResult.error);
}

void handleMessageFromClient(Client &client, size_t messageSize)
{
    if (client.receiveMessage(messageSize))
    {
        const ParseRequestResult parseMessageResult = parseHttpRequest(client.getReceivedMessage(), client.serverPort);
        if (parseMessageResult.status == PARSED || parseMessageResult.status == ERROR)
        {
            client.clearReceivedMessage();
            if (parseMessageResult.status == PARSED)
            {
                const HttpRequest &httpRequest = parseMessageResult.value;
                const ErrorPages &errorPages = CONFIG.getServer(httpRequest.host, client.serverPort).errorPages;
                ImmidiateResponse immidiateResponse =
                    retrieveImmidiateResponse(parseMessageResult.value, client, errorPages);
                if (immidiateResponse.tag == RIGHT)
                    client.appendToBeSentMessage(responseText(immidiateResponse.rightValue));
                else if (immidiateResponse.tag == LEFT)
                {
                    const CgiRequest &cgiRequest = immidiateResponse.leftValue;
                    const CreateCgiResult newCgiResult = createCgi(cgiRequest, errorPages, client, httpRequest);
                    if (newCgiResult.success)
                    {
                        CGIS += newCgiResult.value;
                        CGIS[newCgiResult.value.sd].appendToBeSentMessage(cgiRequest.body);
                    }
                    else
                    {
                        client.appendToBeSentMessage(responseText(newCgiResult.error));
                    }
                }
            }
            else if (parseMessageResult.status == ERROR)
                client.appendToBeSentMessage(responseText(BAD_REQUEST_RESPONSE));
        }
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
                std::cout << "client " << client.sd << " has disconnected" << std::endl;
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
                    eventCgi.getClient()->appendToBeSentMessage(responseText(parseCgiResponseResult.error));
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
            std::cout << "cgi " << eventCgi.sd << " has disconnected" << std::endl;
            close(eventCgi.sd);
            CGIS -= eventCgi;
        }
    }
}
else
{
    utils::printError("kernel event error");
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
                {
                    handleListenSocketEvent(*eventListenSocketIt);
                }
                else if (CLIENTS.has(event.ident))
                {
                    handleClientEvent(event, CLIENTS[event.ident]);
                }
                else if (CGIS.has(event.ident))
                {
                    handleCgiEvent(event, CGIS[event.ident]);
                }
                else
                {
                    utils::printError("unexpected event");
                }
            }
        }
        else
        {
            utils::printError("unexpected event");
        }
    }
}
