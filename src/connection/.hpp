#pragma once
#include "../webserv.hpp"

bool registerReadEvents(const Sockets &sockets);
bool registerEvent(const uintptr_t identifier, const short filter, const short flags);
bool updateEvent(const struct kevent &kevent, const short filter);
void eventLoop(const Sockets &listenSockets);
bool processMessage(Socket &socket);
