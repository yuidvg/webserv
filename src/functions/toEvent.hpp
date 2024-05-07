#pragma once
#include "../types/bottom.hpp"

struct toEvent
{
    const Sockets sockets;

    // Canonical
    toEvent();
    ~toEvent();
    toEvent(const toEvent &other);
    toEvent &operator=(const toEvent &other);

    toEvent(const Sockets &sockets);

    Option<Event> operator()(const struct kevent &kernelEvent) const;
};
