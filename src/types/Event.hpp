#pragma once
#include "Socket.hpp"
#include "external.hpp"

enum EventType
{
    READ,
    WRITE
};

struct Event
{
    const Socket socket;
    const EventType type;
    const intptr_t size;
};
