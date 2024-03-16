#pragma once
#include "../webserv.hpp"

bool createListenSockets(const Servers servers);
NewConnectionResult newConnectedSocket(const Connection &listenSocket);
PortNumberResult portNumber(const Connection sd);
