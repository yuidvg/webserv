#pragma once
#include "Sockets.hpp"

struct ReadableWritableSockets
{
    const Connections::ReadableSocket &readable;
    const Connections::WritableSocket &writable;
};
