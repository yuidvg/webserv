#include "../all.hpp"

namespace
{

bool downCgi(const Socket &cgiSocket)
{
    if (cgiSocket.descriptor != -1 && close(cgiSocket.descriptor) != -1)
    {
        int wstats;
        waitpid(-1, &wstats, WNOHANG);
    }
    return false;
}

} // namespace

void downCgis(const EventDatas &cgiEventDatas)
{
    for (EventDatas::const_iterator it = cgiEventDatas.begin(); it != cgiEventDatas.end(); ++it)
        downCgi((*it).socket);
}

void removeClient(const Socket clientSocket, Outbounds &outbounds)
{
    outbounds.remove(clientSocket);
    if (std::find(SOCKETS.begin(), SOCKETS.end(), clientSocket) != SOCKETS.end())
    {
        SOCKETS.erase(std::find(SOCKETS.begin(), SOCKETS.end(), clientSocket));
    }
    close(clientSocket.descriptor);
}
