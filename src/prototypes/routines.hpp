#pragma once
#include "top.hpp"

void eventLoop();
void handleReceivedMessage();
void processClientMessages(const EventDatas &eventDatas);
void downCgis(const EventDatas &cgiEventDatas);
EventDatas sendEventDatas(const EventDatas &eventDatas);
void writeEventDatas(const EventDatas &eventDatas);
void removeClient(const Socket clientSocket, Outbounds &outbounds);
