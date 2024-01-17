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

typedef Result<const int, const std::string> ListenSocketResult;

struct Socket
{
    const ListenSocketResult listenSocketResult;
    const Server server;

    Socket();
    Socket(const Server server);
};

#endif
