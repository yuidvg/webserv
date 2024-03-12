#include "../socket/.hpp"
#include ".hpp"

void eventLoop()
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
                if (!(event.flags & EV_ERROR))
                {
                    Socket &eventSocket = SOCKETS[event.ident];
                    if (event.filter == EVFILT_READ)
                    {
                        if (!(event.flags & EV_EOF))
                        {
                            if (eventSocket.isListenSocket())
                            {
                                const NewSocketResult newConnectedSocketResult = newConnectedSocket(eventSocket);
                                if (newConnectedSocketResult.success)
                                {
                                    SOCKETS += newConnectedSocketResult.value;
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
                                    processMessage(eventSocket);
                                }
                                else
                                {
                                    utils::printError("failed to receive message: invalid socket.");
                                    close(eventSocket.descriptor);
                                    SOCKETS -= eventSocket;
                                }
                            }
                        }
                        else
                        {
                            std::cout << "socket " << eventSocket.descriptor << " is close\n" << std::endl;
                            close(eventSocket.descriptor);
                            SOCKETS -= eventSocket;
                        }
                    }
                    else if (event.filter == EVFILT_WRITE)
                    {
                        if (!(event.flags & EV_EOF))
                        {
                            if (eventSocket.hasMessageToSend() && !eventSocket.sendMessage(event.data))
                            {
                                std::cout << "socket " << eventSocket.descriptor << " is broken\n" << std::endl;
                                close(eventSocket.descriptor);
                                SOCKETS -= eventSocket;
                            }
                        }
                        else
                        {
                            std::cout << "client " << eventSocket.descriptor << " has disconnected" << std::endl;
                            close(eventSocket.descriptor);
                            SOCKETS -= eventSocket;
                        }
                    }
                }
                else
                {
                    utils::printError("kernel event error");
                }
            }
        }
        else
        {
            utils::printError("unexpected event");
        }
    }
}
