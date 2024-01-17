#include "eventLoop.hpp"

void eventLoop(const Sockets listenSockets)
{
    Sockets connectedSockets;

    while (true)
    {
        const Sockets allSockets = utils::combined(listenSockets, connectedSockets);
        const ReadableSocketsResult readableSocketsResult = readableSockets(allSockets);
        if (!readableSocketsResult.success)
        {
            utils::printError(readableSocketsResult.error);
            // ToDo: close all listenSockets/not.
        }
        Sockets readableSockets = readableSocketsResult.value;
        for (Sockets::iterator readableSocketIt = readableSockets.begin(); readableSocketIt != readableSockets.end();
             ++readableSocketIt)
        {
            if (utils::contained(*readableSocketIt, listenSockets))
            {
                const Socket listenSocket = *readableSocketIt;
                NewSocketResult newConnectedSocketResult = newConnectedSocket(listenSocket);
                if (!newConnectedSocketResult.success)
                {
                    utils::printError(newConnectedSocketResult.error);
                }
                std::cout << "新しい接続を受け入れた" << std::endl;
                connectedSockets.push_back(newConnectedSocketResult.value);
            }
            else
            {
                if (!processConnection(*readableSocketIt))
                {
                    close(readableSocketIt->descriptor);
                    connectedSockets = utils::excluded(connectedSockets, *readableSocketIt);
                }
            }
        }
    }
}
