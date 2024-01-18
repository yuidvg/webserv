#ifndef SOCKET_ALL_HPP
#define SOCKET_ALL_HPP

#include "../webserv.hpp"

// get
NewSocketResult getListenSocket(const Server server);
CreatedSocketsResult getListenSockets(Servers servers);
NewSocketResult newConnectedSocket(const Socket listenSocket);

// find
unsigned int getMaxSd(const Sockets socketsIn, const std::map<int, Socket> &connSocks);
ReadableSocketsResult readableSockets(const Sockets sockets);

#endif
