#pragma once
#include "external.hpp"

struct Socket
{
    const int descriptor;
    const size_t port;
    Socket(const int descriptor, const size_t port = 0) : descriptor(descriptor), port(port)
    {
    }
    Socket() : descriptor(0), port(0)
    {
    }
    Socket(const Socket &x) : descriptor(x.descriptor), port(x.port)
    {
    }
    ~Socket()
    {
    }
    bool operator==(const Socket & x) const
    {
        return descriptor == x.descriptor;
    }
};
