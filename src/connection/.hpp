#pragma once
#include "../webserv.hpp"

bool registerReadEvents(const Sockets &sockets);
bool registerEvent(const uintptr_t identifier, const int16_t filter);
void eventLoop(const Sockets &listenSockets);
ParseStatus processMessage(const Socket &socket);
