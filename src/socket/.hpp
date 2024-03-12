#pragma once
#include "../webserv.hpp"

bool createListenSockets(const Servers servers);
NewSocketResult newConnectedSocket(const Socket &listenSocket);
PortNumberResult portNumber(const Socket sd);
