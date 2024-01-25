#pragma once
#include "../webserv.hpp"

void eventLoop(const Sockets listenSockets, const Servers servers);
bool processConnection(const Socket &socket, const Servers &servers);
