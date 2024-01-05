#include "socket.hpp"

InitializeResult Socket::initialize()
{
    int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        return (InitializeResult::Err("socket() failed"));

    // 同じローカルアドレスとポートを使用しているソケットがあっても、ソケットを再利用できるようにする
    int on = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0)
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
    addr.sin_port = htons(server.port);              // ポート番号を設定

    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
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

int Socket::getListenSocket() const
{
    return (listen_socket);
}

Server Socket::getServer() const
{
    return (server);
}

std::vector<int> Socket::getConnSock() const
{
    return (conn_socks);
}
void Socket::addConnSock(int sock)
{
    conn_socks.push_back(sock);
}

// TODO: conn_sockの中身が削除できていない。
void Socket::deleteConnSock(int sock)
{
    std::vector<int> new_conn_socks;
    bool found = false;

    // Copy all elements except the one to be deleted
    for (std::vector<int>::iterator it = conn_socks.begin(); it != conn_socks.end(); ++it)
    {
        if (*it != sock)
        {
            new_conn_socks.push_back(*it);
        }
        else
        {
            found = true;
        }
    }

    if (found)
    {
        std::cout << YELLOW << "delete ConnSock " << sock << NORMAL << std::endl;
    }
    else
    {
        std::cout << RED << "not found ConnSock " << sock << NORMAL << std::endl;
    }

    // Replace the old vector with the new one
    conn_socks = new_conn_socks;

    // Iterate over the vector
    std::cout << GREEN << "conn_socks: " << std::endl;
    for (std::vector<int>::iterator it = conn_socks.begin(); it != conn_socks.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << NORMAL;
}

Socket::Socket(Server server)
{
    this->server = server;
    InitializeResult result = initialize();
    if (!result.ok())
    {
        std::cout << result.unwrapErr() << std::endl;
        _exit(1);
    }
    listen_socket = result.unwrap();
}