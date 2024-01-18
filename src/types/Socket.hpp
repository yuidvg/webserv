#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Server.hpp"
#include "external.hpp"

struct Socket
{
    const int descriptor;
    const Server server;
    Socket();
    Socket(const unsigned int descriptor, const Server server);
    bool operator==(const Socket other) const;
    bool operator!=(const Socket other) const;
};



#endif
