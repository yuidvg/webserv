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
        for (Sockets::iterator readableSdIt = readableSockets.begin(); readableSdIt != readableSockets.end(); ++readableSdIt)
        {
            if (utils::contains(*readableSdIt, listenSockets))
            {
                const Socket listenSd = *readableSdIt;
                NewListenSocketResult newConnectedSdResult = newConnectedSd(listenSd);
                if (!newConnectedSdResult.success)
                {
                    utils::printError(newConnectedSdResult.error);
                }
                std::cout << "新しい接続を受け入れた" << std::endl;
                connectedSockets.push_back(newConnectedSdResult.value);
            }
            else
            {
                if (!processConnection(*readableSdIt, servers))
                {
                    close(*readableSdIt);
                    connectedSockets = utils::excluded(connectedSockets, *readableSdIt);
                }
            }
        }
    }
}
