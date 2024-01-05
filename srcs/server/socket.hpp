#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../Result/Result.hpp"
#include <vector>

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
    int listen_socket;
    int server_num;
    int port;
    std::vector<int> conn_socks; // この仮想サーバーが受け持つソケットのリスト

public:
    Socket(int port, int server_num);
    InitializeResult initialize();
    int getSocket();
    int getPort();
    int getServerNum();
    void addConnSock(int sock);
    void deleteConnSock(int sock);
    std::vector<int> getConnSock();
};

#endif