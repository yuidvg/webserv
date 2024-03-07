#pragma once
#include "../webserv.hpp"

bool registerReadEvents(const Sockets &sockets);
void eventLoop(const Sockets &listenSockets);
ParseStatus processMessage(const Socket &socket);
