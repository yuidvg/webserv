#pragma once
#include "top.hpp"

void eventLoop(Sockets listenSds);
void handleReceivedMessage();
void handleClientMessages(const ConnectedInternetSocket &socket, const SocketBuffer &socketIO);
