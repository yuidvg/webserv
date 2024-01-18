#ifndef SOCKET_GET_HPP
#define SOCKET_GET_HPP

#include "../types/all.hpp"

NewSocketResult getListenSocket(const Server server);
Sockets getListenSockets(Servers servers);

#endif