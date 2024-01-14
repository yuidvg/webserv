#include "socket.hpp"

Socket::Socket()
    : listenSocket(getListenSocket()), server("", 80, std::map<int, std::string>(), 1048576, std::vector<Location>())
{
}

Socket::~Socket()
{
}

InitializeResult Socket::initialize() const
{
    int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        return (InitializeResult::Error("socket() failed"));

    // 同じローカルアドレスとポートを使用しているソケットがあっても、ソケットを再利用できるようにする
    int on = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
    {
        close(sd);
        return (InitializeResult::Error("setsockopt() failed"));
    }

    int flags = fcntl(sd, F_GETFL, 0);
    if (flags < 0)
    {
        close(sd);
        return (InitializeResult::Error("fcntl() failed"));
    }
    flags |= O_NONBLOCK;
    if (fcntl(sd, F_SETFL, flags) < 0)
    {
        close(sd);
        return (InitializeResult::Error("fcntl() failed to set non-blocking"));
    }
    struct sockaddr_in addr = {};
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // IPv4アドレスを指定
    addr.sin_port = htons(server.port);       // ポート番号を設定

    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sd);
        return (InitializeResult::Error(std::string("bind() failed: " + std::string(strerror(errno)) + "\nポート番号" +
                                                  utils::to_string(server.port))));
    }

    if (listen(sd, 5) < 0)
    {
        close(sd);
        return (InitializeResult::Error("listen() failed"));
    }
    return InitializeResult::Success(sd);
}

int Socket::getListenSocket() const
{
    return (listenSocket);
}

Server Socket::getServer() const
{
    return (server);
}

Socket::Socket(Server server) : listenSocket(-1), server(server)
{
    InitializeResult initializedResult = initialize();
    if (!initializedResult.success)
    {
        std::cout << initializedResult.error << std::endl;
        _exit(1);
    }
    listenSocket = initializedResult.value;
}
