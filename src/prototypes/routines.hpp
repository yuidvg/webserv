#pragma once
#include "top.hpp"

void eventLoop(Sockets listenSds);
void handleReceivedMessage();
void handleClientMessages(const ConnectedInternetSocket &socket, const SocketBuffer &socketIO);
void eraseSocketBuffer(const int sd);
bool appendOutbound(int sd, std::string content);
bool depositCgiRequest(const CgiRequest &cgiRequest);
