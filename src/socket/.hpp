#pragma once
#include "../webserv.hpp"

GetListenSocketsResult getListenSockets(const Servers servers);
NewListenSocketResult newConnectedSocket(const Socket listenSocket);
ReadableSocketsResult readableSockets(const Sockets sds);
PortNumberResult portNumber(const Socket sd);
