#include "../socket/.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"

#include ".hpp"

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
                    const NewConnectionResult newConnectedSocketResult = newConnection(*eventListenSocketIt);
                    if (newConnectedSocketResult.success)
                        CONNECTIONS += newConnectedSocketResult.value;
                    else
                        utils::printError(newConnectedSocketResult.error);
                }
                else
                {
                    if (!(event.flags & EV_ERROR))
                    {
                        Connection &eventConnection = CONNECTIONS[event.ident];
                        if (event.filter == EVFILT_READ)
                        {
                            if (!(event.flags & EV_EOF))
                            {
                                if (eventConnection.receiveMessage(event.data))
                                {
                                    const ParseRequestResult parseMessageResult =
                                        parseHttpRequest(eventConnection.getReceivedMessage(), eventConnection.port);
                                    if (parseMessageResult.status == PARSED || parseMessageResult.status == ERROR)
                                    {
                                        eventConnection.clearReceivedMessage();
                                        if (parseMessageResult.status == PARSED)
                                        {
                                            ImmidiateResponse immidiateResponse =
                                                retrieveImmidiateResponse(parseMessageResult.value, eventConnection);
                                            if (immidiateResponse.tag == RIGHT)
                                                eventConnection.appendToBeSentMessage(
                                                    responseText(immidiateResponse.rightValue));
                                            else if (immidiateResponse.tag == LEFT)
                                            {
                                                const CgiRequest &cgiRequest = immidiateResponse.leftValue;
                                                const HttpResponse cgiResponse =
                                                    executeCgi(cgiRequest, eventConnection, cgiRequest.uri);
                                                eventConnection.appendToBeSentMessage(responseText(cgiResponse));
                                            }
                                        }
                                        else if (parseMessageResult.status == ERROR)
                                            eventConnection.appendToBeSentMessage(responseText(BAD_REQUEST_RESPONSE));
                                    }
                                }
                                else
                                {
                                    utils::printError("failed to receive message: invalid socket.");
                                    close(eventConnection.sd);
                                    CONNECTIONS -= eventConnection;
                                }
                            }
                            else
                            {
                                std::cout << "socket " << eventConnection.sd << " is close\n" << std::endl;
                            }
                        }
                        else if (event.filter == EVFILT_WRITE)
                        {
                            if (!(event.flags & EV_EOF))
                            {
                                if (!eventConnection.sendMessage(event.data))
                                {
                                    std::cerr << "socket " << eventConnection.sd << " is broken\n" << std::endl;
                                    close(eventConnection.sd);
                                    CONNECTIONS -= eventConnection;
                                }
                            }
                            else
                            {
                                std::cout << "client " << eventConnection.sd << " has disconnected" << std::endl;
                                close(eventConnection.sd);
                                CONNECTIONS -= eventConnection;
                            }
                        }
                    }
                    else
                    {
                        utils::printError("kernel event error");
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
