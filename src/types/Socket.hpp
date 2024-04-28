#pragma once
#include "external.hpp"

enum SocketType
{
    LISTEN,
    CLIENT,
    CGI
};

struct Socket
{
    const int descriptor;
    const SocketType type;
    const size_t serverPort;
    const size_t clientPort;
    const std::string clientIp;
    const int opponentPid;

    Socket(const int descriptor, const SocketType type = CLIENT, const size_t serverPort = 0,
           const size_t clientPort = 0, const std::string clientIp = "", const int opponentPid = 0)
        : descriptor(descriptor), type(type), serverPort(serverPort), clientPort(clientPort), clientIp(clientIp),
          opponentPid(opponentPid)
    {
    }
    Socket() : descriptor(0), type(CLIENT), serverPort(0), clientPort(0), clientIp(""), opponentPid(0)
    {
    }
    Socket(const Socket &x)
        : descriptor(x.descriptor), type(x.type), serverPort(x.serverPort), clientPort(x.clientPort),
          clientIp(x.clientIp), opponentPid(x.opponentPid)
    {
    }
    ~Socket()
    {
    }
    bool operator==(const Socket &x) const
    {
        return descriptor == x.descriptor;
    }
    bool operator<(const Socket &x) const
    {
        return descriptor < x.descriptor;
    }
};
