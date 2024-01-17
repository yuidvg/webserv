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


struct Socket
{
    const int descriptor;
    const Server server;
    Socket();
    Socket(const unsigned int descriptor, const Server server);
    bool operator==(const Socket other) const;
    bool operator!=(const Socket other) const;
};

typedef Result<const Socket, const std::string> NewSocketResult;
typedef std::vector<const Socket> Sockets;

NewSocketResult getListenSocket(const Server server);

Sockets getListenSockets(Servers servers);

#endif
