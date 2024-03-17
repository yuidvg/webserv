#pragma once
#include "../webserv.hpp"

SocketsResult createListenSockets(const Servers servers);
NewClientResult newConnection(const Socket &listenSocket);
PortNumberResult portNumber(const Client sd);
