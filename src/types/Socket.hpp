#pragma once

struct Socket
{
    const unsigned int descriptor;
    const unsigned int port;
    const std::string opponentIp;
    const unsigned int opponentPort;

    Socket() : descriptor(0), port(0), opponentIp(0), opponentPort(0){};
    Socket(const unsigned int &descriptor, const unsigned int &port, const std::string &opponentIp = 0,
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
};

typedef std::vector<const Socket> Sockets;
