#pragma once
#include "../webserv.hpp"

void eventLoop(const Sockets &listenSockets);
bool processMessage(const Socket &socket);
