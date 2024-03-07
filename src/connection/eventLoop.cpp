#include "../socket/.hpp"
#include ".hpp"

void eventLoop(const Sockets &listenSockets)
{
    Sockets connectedSockets;
    struct kevent eventList[EVENT_BATCH_SIZE];

    while (true)
    {
        const Sockets allSockets = utils::combined(listenSockets, connectedSockets);
        const int numOfEvents = kevent(KQ, NULL, 0, eventList, EVENT_BATCH_SIZE, NULL);
        if (numOfEvents != -1)
        {
            for (int i = 0; i < numOfEvents; ++i)
            {
                const struct kevent &event = eventList[i];
                if (!(event.flags & EV_ERROR))
                {

                    FindSocketResult eventSocketResult = utils::findSocket(allSockets, event.ident);
                    if (eventSocketResult.success)
                    {
                        Socket eventSocket = eventSocketResult.value;
                        if (event.filter == EVFILT_READ)
                        {
                            if (utils::contains(eventSocket, listenSockets))
                            {
                                const NewSocketResult newConnectedSocketResult = newConnectedSocket(eventSocket);
                                if (newConnectedSocketResult.success)
                                {
                                    std::cout << "new connection socket created." << std::endl;
                                    connectedSockets.push_back(newConnectedSocketResult.value);
                                }
                                else
                                {
                                    utils::printError(newConnectedSocketResult.error);
                                }
                            }
                            else
                            {
                                if (!eventSocket.receiveMessage(event.data))
                                {
                                    utils::printError("failed to receive message: invalid socket.");
                                    close(eventSocket.descriptor);
                                    connectedSockets = utils::excluded(connectedSockets, eventSocket);
                                }
                                else
                                {
                                    const ParseStatus status = processMessage(eventSocket);
                                    if (status == PENDING)
                                    {
                                        continue;
                                    }
                                    else if (status == SUCCESS)
                                    {
                                        std::cout << "message processed." << std::endl;
                                    }
                                    else
                                    {
                                        utils::printError("failed to process message.");
                                    }
                                }
                            }
                        }
                        else if (event.filter == EVFILT_WRITE)
                        {
                            if (!eventSocket.sendMessage(event.data))
                            {
                                utils::printError("failed to send message: invalid socket.");
                                close(eventSocket.descriptor);
                                connectedSockets = utils::excluded(connectedSockets, eventSocket);
                            }
                        }
                    }
                    else
                    {
                        utils::printError(eventSocketResult.error);
                    }
                }
                else
                {
                    utils::printError("kevent error");
                }
            }
        }
        else
        {
            utils::printError("unexpected event");
        }
    }
}
