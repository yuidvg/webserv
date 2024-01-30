#pragma once

struct Socket
{
    const unsigned int descriptor;
    const unsigned int port;
    const std::string opponentIp;
    const unsigned int opponentPort;

    Socket() : descriptor(0), port(0), opponentIp(""), opponentPort(0){};
    Socket(const unsigned int &descriptor, const unsigned int &port, const std::string &opponentIp = "",
           const unsigned int &opponentPort = 0)
        : descriptor(descriptor), port(port), opponentIp(opponentIp), opponentPort(opponentPort){};
    bool operator==(const Socket &other) const
    {
        return descriptor == other.descriptor && port == other.port && opponentIp == other.opponentIp &&
               opponentPort == other.opponentPort;
    };
    bool operator!=(const Socket &other) const
    {
        return !(*this == other);
    };
    Socket(const Socket &other)
    : descriptor(other.descriptor), port(other.port), opponentIp(other.opponentIp), opponentPort(other.opponentPort)
    {
    }
    Socket &operator=(const Socket &other)
    {
        (void)other;
        return *this;
    }
};

typedef std::vector< Socket> Sockets;
