#ifndef SOCKET_ALL_HPP
#define SOCKET_ALL_HPP

#include "../webserv.hpp"

// get
NewListenSdResult getListenSd(const Server server);
GetListenSdsResult getListenSds(Servers servers);
NewListenSdResult newConnectedSd(const Sd listenSd);

// find
ReadableSdsResult readableSds(const Sds sds);

// stats
PortNumberResult portNumber(const Sd sd);

#endif
