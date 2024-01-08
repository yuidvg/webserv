#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../Result/Result.hpp"
#include "../config/parseConfig.hpp"
#include <vector>
#include <cerrno>

#define TRUE 1
#define FALSE 0
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define GREEN "\x1b[32m"
#define NORMAL "\x1b[0m"

typedef Result<int, std::string> InitializeResult;

class Socket
{
private:
    InitializeResult initialize() const;
    int listen_socket;
    // std::vector<int> conn_socks; // connected_sockets この仮想サーバーが受け持つソケットのリスト
    Server server;

public:
    Socket();
    ~Socket();
    Socket(Server server);
    Server getServer() const;
    int getListenSocket() const;
};

#endif