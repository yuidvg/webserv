#include "../socket/.hpp"
#include ".hpp"

void eventLoop(const Sockets listenSockets, const Servers servers)
{
    Sockets connectedSockets;

    while (true)
    {
        const Sockets allSockets = utils::combined(listenSockets, connectedSockets);
        const ReadableSocketsResult readableSocketsResult = readableSockets(allSockets);
        if (!readableSocketsResult.success)
        {
            utils::printError(readableSocketsResult.error);
        }
        Sockets readableSockets = readableSocketsResult.value;
        for (Sockets::iterator readableSocketIt = readableSockets.begin(); readableSocketIt != readableSockets.end(); ++readableSocketIt)
        {
            if (utils::contains(*readableSocketIt, listenSockets))
            {
                const Socket listenSocket = *readableSocketIt;
                NewListenSocketResult newConnectedSocketResult = newConnectedSocket(listenSocket);
                if (!newConnectedSocketResult.success)
                {
                    utils::printError(newConnectedSocketResult.error);
                }
                std::cout << "新しい接続を受け入れた" << std::endl;
                connectedSockets.push_back(newConnectedSocketResult.value);
            }
            else
            {
                if (!processConnection(*readableSocketIt, servers))
                {
                    close(*readableSocketIt);
                    connectedSockets = utils::excluded(connectedSockets, *readableSocketIt);
                }
            }
        }
    }
}
