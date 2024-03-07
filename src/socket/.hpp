#pragma once
#include "../webserv.hpp"

GetListenSocketsResult getListenSockets(const Servers servers);
NewSocketResult newConnectedSocket(const Socket &listenSocket);
PortNumberResult portNumber(const Socket sd);
