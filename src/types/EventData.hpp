#pragma once
#include "external.hpp"

struct EventData
{
    const Socket socket;
    const std::string data;

    EventData() : socket(), data("")
    {
    }
    ~EventData()
    {
    }
    EventData(const Socket &socket, const std::string &data) : socket(socket), data(data)
    {
    }
    EventData(const EventData &other) : socket(other.socket), data(other.data)
    {
    }
    EventData &operator=(const EventData &other)
    {
        if (this != &other)
        {
            const_cast<Socket &>(socket) = other.socket;
            const_cast<std::string &>(data) = other.data;
        }
        return *this;
    }
};
