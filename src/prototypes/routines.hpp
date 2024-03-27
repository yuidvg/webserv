#pragma once
#include "top.hpp"

void eventLoop(Sockets listenSds);
void handleReceivedMessage();
void processClientMessages(const ConnectedInternetSocket &socket, SocketBuffer &socketBuffer);
void eraseSocketBuffer(const int sd);
void appendOutbound(int sd, std::string content);
void depositCgiRequest(const CgiRequest &cgiRequest);
void wipeCgi(const pid_t cgiPid, const int sd);
void processCgiMessage(const ConnectedUnixSocket &socket, const std::string message);