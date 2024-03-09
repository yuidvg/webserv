#include "../socket/.hpp"
#include ".hpp"

void eventLoop(const Sockets &listenSockets)
{
    struct kevent eventList[EVENT_BATCH_SIZE];
    if (registerReadEvents(listenSockets))
    {
        Sockets connectedSockets;
        while (true)
        {
            const Sockets allSockets = utils::combined(listenSockets, connectedSockets);

            std::cout << "waiting for events..." << std::endl;
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
                                // TODO: 関数分割
                                if (utils::contains(eventSocket, listenSockets))
                                {
                                    const NewSocketResult newConnectedSocketResult = newConnectedSocket(eventSocket);
                                    if (newConnectedSocketResult.success)
                                    {
                                        std::cout << "new connection socket created." << std::endl;
                                        connectedSockets.push_back(newConnectedSocketResult.value);
                                        if (!registerEvent(newConnectedSocketResult.value.descriptor, EVFILT_READ,
                                                           EV_ADD))
                                        {
                                            utils::printError("Failed to register read event for new socket");
                                        }
                                    }
                                    else
                                    {
                                        utils::printError(newConnectedSocketResult.error);
                                    }
                                }
                                else
                                {
                                    if (eventSocket.receiveMessage(event.data))
                                    {
                                        if (!processMessage(eventSocket))
                                        {
                                            continue;
                                        }
                                        else
                                        {
                                            std::cout << "message processed." << std::endl;
                                            for (std::vector<Socket>::iterator it = connectedSockets.begin();
                                                 it != connectedSockets.end(); ++it)
                                            {
                                                if (it->descriptor == eventSocket.descriptor)
                                                {
                                                    *it = eventSocket;
                                                    break;
                                                }
                                            }
                                            updateEvent(event, EVFILT_WRITE);
                                        }
                                    }
                                    else
                                    {
                                        utils::printError("failed to receive message: invalid socket.");
                                        close(eventSocket.descriptor);
                                        connectedSockets = utils::excluded(connectedSockets, eventSocket);
                                    }
                                }
                            }
                            else if (event.filter == EVFILT_WRITE)
                            {
                                if (eventSocket.sendMessage(event.data))
                                {
                                    updateEvent(event, EVFILT_READ);
                                }
                                else
                                {
                                    utils::printError("failed to send message: invalid socket.");
                                }
                                std::cout << "socket " << eventSocket.descriptor << " is close" << std::endl;
                                close(eventSocket.descriptor);
                                connectedSockets = utils::excluded(connectedSockets, eventSocket);
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
}
