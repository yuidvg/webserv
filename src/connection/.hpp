#pragma once
#include "../webserv.hpp"

void eventLoop();
bool processMessage(Socket &socket);
