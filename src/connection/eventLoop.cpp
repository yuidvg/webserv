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
                                        // 読み込みイベントを登録 (newConnectedSocketResult.value.descriptor を使って)
                                        if (!registerEvent(newConnectedSocketResult.value.descriptor, EVFILT_READ))
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
                                    if (!eventSocket.receiveMessage(event.data))
                                    {
                                        utils::printError("failed to receive message: invalid socket.");
                                        close(eventSocket.descriptor);
                                        connectedSockets = utils::excluded(connectedSockets, eventSocket);
                                    }
                                    else
                                    {
                                        if (!processMessage(eventSocket))
                                        {
                                            continue;
                                        }
                                        else
                                        {
                                            std::cout << "message processed." << std::endl;
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
}
