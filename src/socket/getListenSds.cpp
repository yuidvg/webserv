#include ".hpp"
namespace
{
NewListenSocketResult getListenSocket(const Server server)
{
    const int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        return (NewListenSocketResult::Error("socket() failed"));

    const int on = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
    {
        close(sd);
        return (NewListenSocketResult::Error("setsockopt() failed"));
    }

    const int nonblockSocketFlags = O_NONBLOCK;
    if (fcntl(sd, F_SETFL, nonblockSocketFlags) < 0)
    {
        close(sd);
        return (NewListenSocketResult::Error("fcntl() failed to set non-blocking"));
    }

    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(server.port);

    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sd);
        return (NewListenSocketResult::Error("bind() failed: "));
        // return (NewListenSocketResult::Error(std::string("bind() failed: " + std::string(strerror(errno)) +
        //                                               "\nポート番号" + utils::to_string(server.port))));
    }

    if (listen(sd, 5) < 0)
    {
        close(sd);
        return (NewListenSocketResult::Error("listen() failed"));
    }
    return NewListenSocketResult::Success(Socket(sd, ntohs(addr.sin_port)));
}
} // namespace
GetListenSocketsResult getListenSockets(Servers servers)
{
    Sockets sockets;
    for (Servers::iterator serverIt = servers.begin(); serverIt != servers.end(); serverIt++)
    {
        NewListenSocketResult newSocketResult = getListenSocket(*serverIt);
        if (newSocketResult.success)
        {
            sockets.push_back(newSocketResult.value);
        }
        else
        {
            return GetListenSocketsResult::Error(newSocketResult.error);
        }
    }
    return GetListenSocketsResult::Success(sockets);
}
