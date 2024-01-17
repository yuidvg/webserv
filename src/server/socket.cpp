#include "socket.hpp"

Socket::Socket()
    : listenSocketResult(ListenSocketResult::Error("Socket not initialized")),
      server("", 80, std::map<int, std::string>(), 1048576, std::vector<Location>())
{
}

Socket::Socket(const Server server) : listenSocketResult(getListenSocket(server)), server(server)
{
}

Socket::~Socket()
{
}

ListenSocketResult getListenSocket(const Server server)
{
    const int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        return (ListenSocketResult::Error("socket() failed"));

    // 同じローカルアドレスとポートを使用しているソケットがあっても、ソケットを再利用できるようにする
    const int on = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
    {
        close(sd);
        return (ListenSocketResult::Error("setsockopt() failed"));
    }

    const int socketFlags = fcntl(sd, F_GETFL, 0);
    if (socketFlags < 0)
    {
        close(sd);
        return (ListenSocketResult::Error("fcntl() failed"));
    }
    const int nonblockSocketFlags = socketFlags | O_NONBLOCK;
    if (fcntl(sd, F_SETFL, nonblockSocketFlags) < 0)
    {
        close(sd);
        return (ListenSocketResult::Error("fcntl() failed to set non-blocking"));
    }
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // IPv4アドレスを指定
    addr.sin_port = htons(server.port);       // ポート番号を設定

    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sd);
        return (ListenSocketResult::Error(std::string("bind() failed: " + std::string(strerror(errno)) +
                                                      "\nポート番号" + utils::to_string(server.port))));
    }

    if (listen(sd, 5) < 0)
    {
        close(sd);
        return (ListenSocketResult::Error("listen() failed"));
    }
    return ListenSocketResult::Success(sd);
}
