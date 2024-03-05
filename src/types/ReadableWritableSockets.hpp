#pragma once
#include "Sockets.hpp"

struct ReadableWritableSockets
{
    const Sockets::ReadableSocket& readable;
    const Sockets::WritableSocket& writable;
};
