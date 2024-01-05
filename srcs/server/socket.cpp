#include "socket.hpp"

InitializeResult Socket::initialize()
{
    int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        return (InitializeResult::Err("socket() failed"));

    // 同じローカルアドレスとポートを使用しているソケットがあっても、ソケットを再利用できるようにする
    int on = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
    {
        close(sd);
        return (InitializeResult::Err("setsockopt() failed"));
    }

    int flags = fcntl(sd, F_GETFL, 0);
    if (flags < 0)
    {
        close(sd);
        return (InitializeResult::Err("fcntl() failed"));
    }
    flags |= O_NONBLOCK;
    if (fcntl(sd, F_SETFL, flags) < 0)
    {
        close(sd);
        return (InitializeResult::Err("fcntl() failed to set non-blocking"));
    }
    struct sockaddr_in addr = {};
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // IPv4アドレスを指定
    addr.sin_port = htons(port);              // ポート番号を設定

    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sd);
        std::cout << errno << std::endl;
        return (InitializeResult::Err("bind() failed"));
    }

    if (listen(sd, 5) < 0)
    {
        close(sd);
        return (InitializeResult::Err("listen() failed"));
    }

    return InitializeResult::Ok(sd);
}

int Socket::getSocket()
{
    return (listen_socket);
}
int Socket::getPort()
{
    return (port);
}

Socket::Socket(int port)
{
    this->port = port;
    InitializeResult result = initialize();
    if (!result.ok())
    {
        std::cout << result.unwrapErr() << std::endl;
        exit(1);
    }
    listen_socket = result.unwrap();
}