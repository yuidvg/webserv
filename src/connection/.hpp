#pragma once
#include "../webserv.hpp"

void eventLoop(const Sockets listenSockets);
bool processConnection(const Socket &socket);
