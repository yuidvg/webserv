#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../config/parseConfig.hpp"
#include "../utils/utils.hpp"
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
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
    InitializeResult initialize() const;
    int listenSocket;
    const Server server;

  public:
    Socket();
    Socket(Server server);
    ~Socket();
    Server getServer() const;
    int getListenSocket() const;
};

#endif
