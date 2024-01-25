#pragma once
#include "../webserv.hpp"

GetListenSocketsResult getListenSockets(const Servers servers);
NewListenSocketResult newConnectedSd(const Socket listenSd);
ReadableSocketsResult readableSockets(const Sockets sds);
PortNumberResult portNumber(const Socket sd);
