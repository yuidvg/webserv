#include "all.hpp"

NewListenSdResult newConnectedSd(const Sd listenSd)
{
    const int newSd = accept(static_cast<int>(listenSd), NULL, NULL);
    if (newSd < 0)
    {
        return NewListenSdResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
    std::cout << "New conected socket: " << newSd << std::endl;
    return NewListenSdResult::Success(newSd);
}

NewListenSdResult getListenSd(const Server server)
{
    const int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        return (NewListenSdResult::Error("socket() failed"));

    const int on = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
    {
        close(sd);
        return (NewListenSdResult::Error("setsockopt() failed"));
    }

    const int nonblockSocketFlags = O_NONBLOCK;
    if (fcntl(sd, F_SETFL, nonblockSocketFlags) < 0)
    {
        close(sd);
        return (NewListenSdResult::Error("fcntl() failed to set non-blocking"));
    }

    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(server.port);

    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sd);
        return (NewListenSdResult::Error("bind() failed: "));
        // return (NewListenSdResult::Error(std::string("bind() failed: " + std::string(strerror(errno)) +
        //                                               "\nポート番号" + utils::to_string(server.port))));
    }

    if (listen(sd, 5) < 0)
    {
        close(sd);
        return (NewListenSdResult::Error("listen() failed"));
    }
    return NewListenSdResult::Success(sd);
}

GetListenSdsResult getListenSds(Servers servers)
{
    Sds sds;
    for (Servers::iterator serverIt = servers.begin(); serverIt != servers.end(); serverIt++)
    {
        NewListenSdResult newSdResult = getListenSd(*serverIt);
        if (newSdResult.success)
        {
            sds.push_back(newSdResult.value);
        }
        else
        {
            return GetListenSdsResult::Error(newSdResult.error);
        }
    }
    return GetListenSdsResult::Success(sds);
}
