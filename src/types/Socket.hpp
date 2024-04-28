#pragma once
#include "external.hpp"

struct Socket
{
    const int descriptor;
    const size_t serverPort;
    const size_t clientPort;
    const std::string clientIp;
    const int opponentPid;

    Socket(const int descriptor, const size_t serverPort, const size_t clientPort, const std::string clientIp,
           const int opponentPid)
        : descriptor(descriptor), serverPort(serverPort), clientPort(clientPort), clientIp(clientIp),
          opponentPid(opponentPid)
    {
    }
    Socket() : descriptor(0), serverPort(0), clientPort(0), clientIp(""), opponentPid(0)
    {
    }
    Socket(const Socket &x)
        : descriptor(x.descriptor), serverPort(x.serverPort), clientPort(x.clientPort), clientIp(x.clientIp),
          opponentPid(x.opponentPid)
    {
    }
    ~Socket()
    {
    }
    bool operator==(const Socket &x) const
    {
        return descriptor == x.descriptor;
    }
};
