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

typedef Result<const Socket, const std::string> NewSocketResult;
typedef std::vector<Socket> Sockets;

struct Socket
{
    const int descriptor;
    const Server server;
    Socket();
    Socket(const Server server);
};

NewSocketResult getListenSocketDescriptor(const Server server);


#endif
