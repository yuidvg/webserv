#pragma once
#include "../webserv.hpp"

SocketsResult createListenSockets(const Servers servers);
ConnectedInternetSocketResult newConnectedInternetSocket(const Socket &listenSocket);
