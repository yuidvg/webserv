#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../Result/Result.hpp"

typedef Result<int, std::string> InitializeResult;

class Socket
{
private:
    int listen_socket;
    int port;

public:
    Socket(int port);
    InitializeResult initialize();
    int getSocket();
    int getPort();
};

#endif