#pragma once
#include "external.hpp"

struct ConnectedUnixSocket
{
    const int descriptor;
    const int opponentPid;
    ConnectedUnixSocket(const int descriptor, const int opponentPid) : descriptor(descriptor), opponentPid(opponentPid)
    {
    }
    ConnectedUnixSocket() : descriptor(0), opponentPid(0)
    {
    }
    ConnectedUnixSocket(const ConnectedUnixSocket &x) : descriptor(x.descriptor), opponentPid(x.opponentPid)
    {
    }
    ~ConnectedUnixSocket()
    {
    }
    bool operator==(const ConnectedUnixSocket &x) const
    {
        return descriptor == x.descriptor;
    }
};
