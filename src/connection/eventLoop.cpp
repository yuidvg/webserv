#include "../socket/.hpp"
#include ".hpp"

namespace
{
respondToReadEvent(Connection &eventSocket)
{
    if (eventSocket.receiveMessage(event.data) && processedMessage(eventSocket))
    {
    }
    else
    {
        utils::printError("failed to receive message: invalid socket.");
        close(eventSocket.sd);
        CONNECTIONS -= eventSocket;
    }
}
} // namespace

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
                    Connection &eventSocket = CONNECTIONS[event.ident];
                    if (event.filter == EVFILT_READ)
                    {
                        if (!(event.flags & EV_EOF))
                        {
                            if (eventSocket.isListenSocket())
                            {
                                const NewConnectionResult newConnectedSocketResult = newConnectedSocket(eventSocket);
                                if (newConnectedSocketResult.success)
                                {
                                    CONNECTIONS += newConnectedSocketResult.value;
                                }
                                else
                                {
                                    utils::printError(newConnectedSocketResult.error);
                                }
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                            std::cout << "socket " << eventSocket.sd << " is close\n" << std::endl;
                        }
                    }
                    else if (event.filter == EVFILT_WRITE)
                    {
                        if (!(event.flags & EV_EOF))
                        {
                            if (!eventSocket.sendMessage(event.data))
                            {
                                std::cout << "socket " << eventSocket.sd << " is broken\n" << std::endl;
                                close(eventSocket.sd);
                                CONNECTIONS -= eventSocket;
                            }
                        }
                        else
                        {
                            std::cout << "client " << eventSocket.sd << " has disconnected" << std::endl;
                            close(eventSocket.sd);
                            CONNECTIONS -= eventSocket;
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
