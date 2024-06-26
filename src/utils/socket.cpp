#include "../all.hpp"

namespace
{
SocketResult getListenSocket(const Server server)
{
    const int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        return (SocketResult::Error("socket() failed"));

    const int on = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
    {
        close(sd);
        return (SocketResult::Error("setsockopt() failed"));
    }

    const int nonblockSocketFlags = O_NONBLOCK;
    if (fcntl(sd, F_SETFL, nonblockSocketFlags) < 0)
    {
        close(sd);
        return (SocketResult::Error("fcntl() failed to set non-blocking"));
    }

    struct sockaddr_in sin;
    sin.sin_family = PF_INET;
    sin.sin_port = htons(server.port);
    sin.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        close(sd);
        // return (SocketResult::Error("bind() failed: "));
        return (SocketResult::Error(std::string("bind() failed: " + std::string(strerror(errno)) + "\nポート番号" +
                                                std::to_string(server.port))));
    }

    if (listen(sd, SOMAXCONN) < 0)
    {
        close(sd);
        return (SocketResult::Error("listen() failed"));
    }

    if (!utils::registerEvent(sd, EVFILT_READ))
    {
        close(sd);
        return (SocketResult::Error("failed to register listen socket"));
    }

    return SocketResult::Success(Socket(sd, INITIATE, server.port));
}
} // namespace

namespace utils
{

SocketsResult createListenSockets(const Servers &servers)
{
    std::set< const int > openedPorts;
    Sockets listenSockets;
    for (Servers::const_iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt)
    {
        if (openedPorts.find(serverIt->port) == openedPorts.end())
        {
            SocketResult newSocketResult = getListenSocket(*serverIt);
            if (newSocketResult.success)
            {
                openedPorts.insert(serverIt->port);
                listenSockets.insert(newSocketResult.value);
            }
            else
            {
                return SocketsResult::Error(newSocketResult.error);
            }
        }
    }
    return SocketsResult::Success(listenSockets);
}

SocketResult newClientSocket(const Socket &listenSocket)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    const int newSd = accept(listenSocket.descriptor, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (newSd >= 0)
    {
        if (utils::registerEvent(newSd, EVFILT_READ) && utils::registerEvent(newSd, EVFILT_WRITE) &&
            utils::setEventFlags(newSd, EVFILT_WRITE, EV_DISABLE))
        {
            return SocketResult::Success(Socket(newSd, CLIENT, listenSocket.serverPort, ntohs(clientAddr.sin_port),
                                                std::string(inet_ntoa(clientAddr.sin_addr)), 0));
        }
        else
        {
            close(newSd);
            return SocketResult::Error("Failed to register new conected socket: " + std::string(strerror(errno)));
        }
    }
    else
    {
        return SocketResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
}

Sockets newClientSockets(const Events &initiateEvents)
{
    Sockets newClientSockets;
    for (Events::const_iterator it = initiateEvents.begin(); it != initiateEvents.end(); ++it)
    {
        const Event event = *it;
        for (intptr_t i = 0; i < event.size; i++)
        {
            const SocketResult newClientSocketResult = newClientSocket(it->socket);
            if (newClientSocketResult.success)
            {
                newClientSockets.insert(newClientSocketResult.value);
            }
            else
            {
                utils::printError(newClientSocketResult.error);
            }
        }
    }
    return newClientSockets;
}
} // namespace utils
