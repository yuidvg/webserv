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
    Event(const Socket &socket, const EventType type, const intptr_t size) : socket(socket), type(type), size(size)
    {
    }
    ~Event()
    {
    }
    Event() : socket(Socket()), type(READ), size(0)
    {
    }
    Event(const Event &other) : socket(other.socket), type(other.type), size(other.size)
    {
    }
    bool operator==(const Event &other) const
    {
        return socket == other.socket && type == other.type && size == other.size;
    }
    bool operator!=(const Event &other) const
    {
        return !(*this == other);
    }
    bool operator<(const Event &other) const
    {
        return socket < other.socket;
    }
};
