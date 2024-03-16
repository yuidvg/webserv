#include ".hpp"
namespace
{
NewConnectionResult getListenSocket(const Server server)
{
    const int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        return (NewConnectionResult::Error("socket() failed"));

    const int on = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
    {
        close(sd);
        return (NewConnectionResult::Error("setsockopt() failed"));
    }

    const int nonblockSocketFlags = O_NONBLOCK;
    if (fcntl(sd, F_SETFL, nonblockSocketFlags) < 0)
    {
        close(sd);
        return (NewConnectionResult::Error("fcntl() failed to set non-blocking"));
    }

    struct sockaddr_in sin;
    sin.sin_family = PF_INET;
    sin.sin_port = htons(server.port);
    sin.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        close(sd);
        // return (NewConnectionResult::Error("bind() failed: "));
        return (NewConnectionResult::Error(std::string("bind() failed: " + std::string(strerror(errno)) +
                                                       "\nポート番号" + std::to_string(server.port))));
    }

    if (listen(sd, SOMAXCONN) < 0)
    {
        close(sd);
        return (NewConnectionResult::Error("listen() failed"));
    }

    if (!utils::registerEvent(sd, EVFILT_READ))
    {
        close(sd);
        return (NewConnectionResult::Error("failed to register listen socket"));
    }

    return NewConnectionResult::Success(Connection(sd, ntohs(sin.sin_port)));
}
} // namespace

bool createListenSockets(Servers servers)
{
    std::set<int> openedPorts;
    for (Servers::iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt)
    {
        if (openedPorts.find(serverIt->port) != openedPorts.end())
            continue;
        NewConnectionResult newSocketResult = getListenSocket(*serverIt);
        if (newSocketResult.success)
        {
            SOCKETS += newSocketResult.value;
            openedPorts.insert(serverIt->port);
        }
        else
        {
            return false;
        }
    }
    return true;
}
