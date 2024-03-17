#pragma once
#include "../webserv.hpp"

SocketsResult createListenSockets(const Servers servers);
NewConnectionResult newConnection(const Socket &listenSocket);
PortNumberResult portNumber(const Connection sd);
