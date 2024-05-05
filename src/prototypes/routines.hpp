#pragma once
#include "top.hpp"

void eventLoop(Sockets listenSds);
void handleReceivedMessage();
void processClientMessages(const EventDatas &eventDatas);
bool killCgi(const Socket &cgiSocket);
EventDatas sendEventDatas(const Events &events, const EventDatas &eventDatas);
