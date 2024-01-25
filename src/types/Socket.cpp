#include "Socket.hpp"

Socket::Socket() : descriptor(0), port(0), opponentIp(0), opponentPort(0)
{
}

Socket::Socket(const unsigned int &descriptor, const unsigned int &port, const unsigned int &opponentIp,
               const unsigned int &opponentPort)
    : descriptor(descriptor), port(port), opponentIp(opponentIp), opponentPort(opponentPort)
{
}
