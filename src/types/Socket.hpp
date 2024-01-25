#pragma once

struct Socket
{
    const unsigned int descriptor;
    const unsigned int port;
    const unsigned int opponentIp;
    const unsigned int opponentPort;

    Socket();
    Socket(const unsigned int &descriptor, const unsigned int &port, const unsigned int &opponentIp = 0,
           const unsigned int &opponentPort = 0);
};
