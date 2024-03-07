#pragma once
#include "../webserv.hpp"

void eventLoop(const Sockets &listenSockets);
ParseStatus processMessage(const Socket &socket);
