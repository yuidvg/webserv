#pragma once
#include "../webserv.hpp"

bool registerReadEvents(const Sockets &sockets);
bool registerEvent(const uintptr_t identifier, const short filter, const short flags);
bool updateEvent(const uintptr_t identifier, const short filter, const short flags);
void eventLoop(const Sockets &listenSockets);
bool processMessage(Socket &socket);
