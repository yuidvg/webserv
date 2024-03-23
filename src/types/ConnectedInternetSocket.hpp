#pragma once
#include "external.hpp"

struct ConnectedInternetSocket
{
    const int descriptor;
    const size_t serverPort;
    const size_t clientPort;
    const std::string clientIp;
    ConnectedInternetSocket(const int descriptor, const size_t serverPort, const size_t clientPort, const std::string clientIp)
        : descriptor(descriptor), serverPort(serverPort), clientPort(clientPort), clientIp(clientIp)
    {
    }
    ConnectedInternetSocket() : descriptor(0), serverPort(0), clientPort(0), clientIp("")
    {
    }
    ConnectedInternetSocket(const ConnectedInternetSocket &x)
        : descriptor(x.descriptor), serverPort(x.serverPort), clientPort(x.clientPort), clientIp(x.clientIp)
    {
    }
    ~ConnectedInternetSocket()
    {
    }
    bool operator==(const ConnectedInternetSocket &x) const
    {
        return descriptor == x.descriptor;
    }
};
