#pragma once
#include "../webserv.hpp"

void eventLoop(const Sds listenSds, const Servers servers);
bool processConnection(const Sd &socket, const Servers &servers);
