#include "socket.hpp"

Socket::Socket()
    : descriptor(-1), server("", 80, std::map<int, std::string>(), 1048576, std::vector<Location>())
{
}

Socket::Socket(const Server server): descriptor(getListenSocket(server)), server(server)
{
}


NewSocketResult getListenSocketDescriptor(const Server server)
{
    const int sD = socket(PF_INET, SOCK_STREAM, 0);
    if (sD < 0)
        return (NewSocketResult::Error("socket() failed"));

    // 同じローカルアドレスとポートを使用しているソケットがあっても、ソケットを再利用できるようにする
    const int on = 1;
    if (setsockopt(sD, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
    {
        close(sD);
        return (NewSocketResult::Error("setsockopt() failed"));
    }

    const int socketFlags = fcntl(sD, F_GETFL, 0);
    if (socketFlags < 0)
    {
        close(sD);
        return (NewSocketResult::Error("fcntl() failed"));
    }
    const int nonblockSocketFlags = socketFlags | O_NONBLOCK;
    if (fcntl(sD, F_SETFL, nonblockSocketFlags) < 0)
    {
        close(sD);
        return (NewSocketResult::Error("fcntl() failed to set non-blocking"));
    }
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // IPv4アドレスを指定
    addr.sin_port = htons(server.port);       // ポート番号を設定

    if (bind(sD, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sD);
        return (NewSocketResult::Error("bind() failed: "));
        // return (NewSocketResult::Error(std::string("bind() failed: " + std::string(strerror(errno)) +
        //                                               "\nポート番号" + utils::to_string(server.port))));
    }
    if (listen(sD, 5) < 0)
    {
        close(sD);
        return (NewSocketResult::Error("listen() failed"));
    }
    return NewSocketResult::Success(Socket());
}
