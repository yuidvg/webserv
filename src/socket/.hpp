#pragma once
#include "../webserv.hpp"

GetListenSocketsResult getListenSockets(const Servers servers);
NewSocketResult newConnectedSocket(const Socket listenSocket);
ReadableWritableSocketsResult readableOrWritableSockets(const Sockets sds);
PortNumberResult portNumber(const Socket sd);
