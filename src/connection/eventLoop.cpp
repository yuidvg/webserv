#include "eventLoop.hpp"
#include "../socket/all.hpp"
#include "process.hpp"

void eventLoop(const Sds listenSds, const Servers servers)
{
    Sds connectedSds;

    while (true)
    {
        const Sds allSds = utils::combined(listenSds, connectedSds);
        const ReadableSdsResult readableSdsResult = readableSds(allSds);
        if (!readableSdsResult.success)
        {
            utils::printError(readableSdsResult.error);
        }
        Sds readableSds = readableSdsResult.value;
        for (Sds::iterator readableSdIt = readableSds.begin(); readableSdIt != readableSds.end();
             ++readableSdIt)
        {
            if (utils::contained(*readableSdIt, listenSds))
            {
                const Sd listenSd = *readableSdIt;
                NewListenSdResult newConnectedSdResult = newConnectedSd(listenSd);
                if (!newConnectedSdResult.success)
                {
                    utils::printError(newConnectedSdResult.error);
                }
                std::cout << "新しい接続を受け入れた" << std::endl;
                connectedSds.push_back(newConnectedSdResult.value);
            }
            else
            {
                if (!processConnection(*readableSdIt, servers))
                {
                    close(*readableSdIt);
                    connectedSds = utils::excluded(connectedSds, *readableSdIt);
                }
            }
        }
    }
}
