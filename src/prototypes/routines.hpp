#pragma once
#include "top.hpp"

void eventLoop(Sockets listenSds);
void handleReceivedMessage();
void processClientMessages(const EventDatas &eventDatas);
bool killCgi(const Socket &cgiSocket);
EventDatas sendEventDatas(const EventDatas &eventDatas);
HttpResponses writeEventDatas(const EventDatas &eventDatas);
