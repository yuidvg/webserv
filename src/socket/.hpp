#pragma once
#include "../types/.hpp"

SocketsResult createListenSockets(const Servers servers);
ConnectedInternetSocketResult newConnectedInternetSocket(const Socket &listenSocket);
