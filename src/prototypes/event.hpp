#pragma once
#include ".hpp"

void eventLoop(Sockets listenSds);
void handleReceivedMessage();
void handleClientMessages(const ConnectedInternetSocket &socket, const SocketBuffer &socketIO);
