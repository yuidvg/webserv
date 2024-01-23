#pragma once
#include "../webserv.hpp"

GetListenSdsResult getListenSds(const Servers servers);
NewListenSdResult newConnectedSd(const Sd listenSd);
ReadableSdsResult readableSds(const Sds sds);
PortNumberResult portNumber(const Sd sd);
