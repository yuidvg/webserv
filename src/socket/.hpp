#pragma once
#include "../webserv.hpp"

bool createListenSockets(const Servers servers);
NewConnectionResult newConnection(const Connection &listenSocket);
PortNumberResult portNumber(const Connection sd);
